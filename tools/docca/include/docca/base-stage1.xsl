<!DOCTYPE xsl:stylesheet [
<!-- TODO: complete this list -->
<!ENTITY BLOCK_LEVEL_ELEMENT "programlisting
                            | itemizedlist
                            | orderedlist
                            | parameterlist
                            | simplesect
                            | para
                            | table
                            | linebreak">
]>
<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  exclude-result-prefixes="xs d"
  expand-text="yes">

  <xsl:include href="common.xsl"/>

  <xsl:output indent="yes"/>

  <xsl:template match="/doxygen" priority="1">
    <page id="{@d:page-id}" type="{@d:page-type}">
      <xsl:apply-templates mode="index-term-atts" select="."/>
      <title>
        <xsl:apply-templates mode="page-title" select="."/>
      </title>
      <xsl:apply-templates select="@d:base-compound-refid"/>
      <xsl:next-match/>
    </page>
  </xsl:template>

          <xsl:template match="@d:base-compound-refid">
            <div>(Inherited from <ref d:refid="{.}">{../@d:base-compound-name}</ref>)</div>
          </xsl:template>

          <!-- Put an index term on every page except class (compound) and overloaded-member pages -->
          <xsl:template mode="index-term-atts" match="doxygen[@d:page-type eq 'compound' or @d:overload-position]"/>
          <xsl:template mode="index-term-atts" match="doxygen">
            <xsl:attribute name="primary-index-term">
              <xsl:apply-templates mode="primary-index-term" select="."/>
            </xsl:attribute>
            <xsl:apply-templates mode="secondary-index-term-att" select="."/>
          </xsl:template>

                  <!-- By default, use the member name as the primary term... -->
                  <xsl:template mode="primary-index-term" match="doxygen">
                    <xsl:apply-templates mode="member-name" select="."/>
                  </xsl:template>
                  <!-- ...and the compound name as the secondary term. -->
                  <xsl:template mode="secondary-index-term-att" match="doxygen">
                    <xsl:attribute name="secondary-index-term">
                      <xsl:apply-templates mode="compound-name" select="."/>
                    </xsl:attribute>
                  </xsl:template>

                  <!-- But with namespace members, use the fully-qualified name as the primary term... -->
                  <xsl:template mode="primary-index-term" match="doxygen[compounddef/@kind eq 'namespace']">
                    <xsl:apply-templates mode="compound-and-member-name" select="."/>
                  </xsl:template>
                  <!-- ...and no secondary term. -->
                  <xsl:template mode="secondary-index-term-att" match="doxygen[compounddef/@kind eq 'namespace']"/>

                          <xsl:template mode="compound-name" match="doxygen"
                            >{d:strip-doc-ns(compounddef/compoundname)}</xsl:template>

                          <xsl:template mode="member-name" match="doxygen"
                            >{(compounddef/sectiondef/memberdef/name)[1]}</xsl:template>

                          <xsl:template mode="compound-and-member-name" match="doxygen">
                            <xsl:variable name="compound-name" as="xs:string">
                              <xsl:apply-templates mode="compound-name" select="."/>
                            </xsl:variable>
                            <xsl:if test="$compound-name">{$compound-name}::</xsl:if>
                            <xsl:apply-templates mode="member-name" select="."/>
                          </xsl:template>

          <xsl:template mode="page-title" match="doxygen[@d:page-type eq 'compound']">
            <xsl:apply-templates mode="compound-name" select="."/>
          </xsl:template>
          <xsl:template mode="page-title" match="doxygen">
            <xsl:apply-templates mode="compound-and-member-name" select="."/>
            <xsl:apply-templates mode="overload-qualifier" select="."/>
          </xsl:template>

                  <xsl:template mode="overload-qualifier" match="doxygen"/>
                  <xsl:template mode="overload-qualifier" match="doxygen[@d:overload-position]">
                    <xsl:text> (</xsl:text>
                    <xsl:value-of select="@d:overload-position"/>
                    <xsl:text> of </xsl:text>
                    <xsl:value-of select="@d:overload-size"/>
                    <xsl:text> overloads)</xsl:text>
                  </xsl:template>


  <xsl:template match="/doxygen[@d:page-type eq 'compound']">
    <xsl:apply-templates select="compounddef"/>
  </xsl:template>

  <xsl:template match="/doxygen[@d:page-type eq 'member']">
    <xsl:apply-templates select="compounddef/sectiondef/memberdef"/> <!-- should just be one -->
  </xsl:template>

  <xsl:template match="/doxygen[@d:page-type eq 'overload-list']">
    <xsl:apply-templates select="(compounddef/sectiondef/memberdef)[1]"/>
  </xsl:template>

  <xsl:template match="compounddef | memberdef" priority="2">
    <xsl:next-match/>
    <xsl:apply-templates mode="includes" select=".">
      <xsl:with-param name="is-footer" select="true()"/>
    </xsl:apply-templates>
  </xsl:template>

  <!-- For convenience, pre-calculate some member sequences and tunnel them through -->
  <xsl:template match="compounddef" priority="1">
    <xsl:next-match>
      <xsl:with-param name="public-types"
                      select="sectiondef[@kind eq 'public-type']/memberdef
                            | innerclass[@prot eq 'public'][not(d:should-ignore-inner-class(.))]"
                      tunnel="yes"/>
      <xsl:with-param name="friends"
                      select="sectiondef[@kind eq 'friend']/memberdef[not(type = ('friend class','friend struct'))]
                                                                     [not(d:should-ignore-friend(.))]"
                      tunnel="yes"/>
    </xsl:next-match>
  </xsl:template>

  <xsl:template match="compounddef">
    <xsl:param name="public-types" tunnel="yes"/>
    <xsl:param name="friends" tunnel="yes"/>

    <xsl:apply-templates select="briefdescription"/>

    <xsl:apply-templates mode="section"
                         select=".,

                                 ( $public-types/self::memberdef/..
                                 | $public-types/self::innerclass
                                 )[1],

                                 sectiondef[@kind = (   'public-func',   'public-static-func')],
                                 sectiondef[@kind = ('protected-func','protected-static-func')],
                                 sectiondef[@kind = (  'private-func',  'private-static-func')][$include-private-members],

                                 sectiondef[@kind = (   'public-attrib',   'public-static-attrib')],
                                 sectiondef[@kind = ('protected-attrib','protected-static-attrib')],
                                 sectiondef[@kind = (  'private-attrib',  'private-static-attrib')][$include-private-members],

                                 $friends/..,

                                 sectiondef[@kind eq 'related'],

                                 detaileddescription
                                 "/>
  </xsl:template>

  <xsl:template match="memberdef">
    <xsl:apply-templates select="briefdescription"/>
    <xsl:apply-templates mode="section" select="., detaileddescription"/>
  </xsl:template>

  <xsl:template match="memberdef[@kind eq 'enum']">
    <xsl:apply-templates select="briefdescription"/>
    <xsl:apply-templates mode="section" select="., parent::sectiondef, detaileddescription"/>
  </xsl:template>

  <xsl:template match="memberdef[/doxygen/@d:page-type eq 'overload-list']">
    <xsl:for-each-group select="../../sectiondef/memberdef" group-by="briefdescription">
      <xsl:apply-templates select="briefdescription"/>
      <xsl:apply-templates mode="overload-list" select="current-group()"/>
    </xsl:for-each-group>
  </xsl:template>

          <xsl:template mode="overload-list" match="memberdef">
            <overloaded-member>
              <xsl:apply-templates mode="normalize-params" select="templateparamlist"/>
              <xsl:apply-templates mode="modifier" select="(@explicit, @friend, @static)[. eq 'yes'],
                                                           @virt[. eq 'virtual']"/>
              <xsl:apply-templates select="type"/>
              <ref d:refid="{@d:page-refid}">{name}</ref>
              <params>
                <xsl:apply-templates select="param"/>
              </params>
              <xsl:apply-templates mode="modifier" select="@const[. eq 'yes']"/>
            </overloaded-member>
          </xsl:template>

                  <xsl:template mode="modifier" match="@*">
                    <modifier>{local-name(.)}</modifier>
                  </xsl:template>
                  <xsl:template mode="modifier" match="@virt">
                    <modifier>virtual</modifier>
                  </xsl:template>


  <xsl:template match="type">
    <type>
      <xsl:value-of select="d:cleanup-type(normalize-space(.))"/>
    </type>
  </xsl:template>

  <!-- d:cleanup-param() may not be needed, and the above may suffice. (TODO: confirm this and remove d:cleanup-param() if so)
  <xsl:template match="param/type">
    <type>
      <xsl:value-of select="d:cleanup-param(.)"/>
    </type>
  </xsl:template>
  -->

  <!-- TODO: Should this be a custom rule or built-in? -->
  <xsl:template mode="section" match="simplesect[matches(title,'Concepts:?')]"/>

  <!-- Omit description section if it has no body -->
  <xsl:template mode="section" match="detaileddescription[not(normalize-space(.))]" priority="1"/>

  <xsl:template mode="section" match="*">
    <section>
      <heading>
        <xsl:apply-templates mode="section-heading" select="."/>
      </heading>
      <xsl:apply-templates mode="section-body" select="."/>
    </section>
  </xsl:template>

  <xsl:template match="simplesect | parameterlist">
    <xsl:apply-templates mode="section" select="."/>
  </xsl:template>

  <xsl:template mode="section-heading" match="memberdef |
                                              compounddef        ">Synopsis</xsl:template>
  <xsl:template mode="section-heading" match="detaileddescription">Description</xsl:template>

  <xsl:template mode="section-heading" match="simplesect[@kind eq 'note'  ]">Remarks</xsl:template>
  <xsl:template mode="section-heading" match="simplesect[@kind eq 'see'   ]">See Also</xsl:template>
  <xsl:template mode="section-heading" match="simplesect[@kind eq 'return']">Return Value</xsl:template>
  <xsl:template mode="section-heading" match="simplesect"                   >{title}</xsl:template>

  <xsl:template mode="section-heading" match="parameterlist[@kind eq 'exception'    ]">Exceptions</xsl:template>
  <xsl:template mode="section-heading" match="parameterlist[@kind eq 'templateparam']">Template Parameters</xsl:template>
  <xsl:template mode="section-heading" match="parameterlist                          ">Parameters</xsl:template>

  <xsl:template mode="section-heading" match="innerclass">Types</xsl:template>

  <xsl:template mode="section-heading" match="sectiondef[@kind eq 'friend' ]">Friends</xsl:template>
  <xsl:template mode="section-heading" match="sectiondef[@kind eq 'related']">Related Functions</xsl:template>
  <xsl:template mode="section-heading" match="sectiondef[@kind eq 'enum'   ]">Values</xsl:template>

  <xsl:template mode="section-heading" match="sectiondef">
    <xsl:apply-templates mode="access-level"  select="@kind"/>
    <xsl:apply-templates mode="storage-class" select="@kind"/>
    <xsl:apply-templates mode="member-kind"  select="@kind"/>
  </xsl:template>

          <xsl:template mode="access-level" match="@kind[starts-with(.,'public-'   )]"/>
          <xsl:template mode="access-level" match="@kind[starts-with(.,'protected-')]">Protected </xsl:template>
          <xsl:template mode="access-level" match="@kind[starts-with(.,'private-'  )]">Private </xsl:template>

          <xsl:template mode="storage-class" match="@*"/>
          <xsl:template mode="storage-class" match="@kind[contains(.,'-static-')]">Static </xsl:template>

          <xsl:template mode="member-kind" priority="1"
                                           match="@kind[ends-with(.,'-static-attrib')]">Members</xsl:template>
          <xsl:template mode="member-kind" match="@kind[ends-with(.,'-attrib'       )]">Data Members</xsl:template>
          <xsl:template mode="member-kind" match="@kind[ends-with(.,'-func'         )]">Member Functions</xsl:template>
          <xsl:template mode="member-kind" match="@kind[ends-with(.,'-type'         )]">Types</xsl:template>


  <xsl:template mode="section-body" match="sectiondef | innerclass | parameterlist">
    <table>
      <tr>
        <th>
          <xsl:apply-templates mode="column-1-name" select="."/>
        </th>
        <th>
          <xsl:apply-templates mode="column-2-name" select="."/>
        </th>
      </tr>
      <xsl:apply-templates mode="table-body" select="."/>
    </table>
  </xsl:template>

          <xsl:template mode="column-1-name" match="*">Name</xsl:template>
          <xsl:template mode="column-2-name" match="*">Description</xsl:template>

          <xsl:template mode="column-1-name"
                        match="parameterlist[@kind = ('exception','templateparam')]">Type</xsl:template>

          <xsl:template mode="column-2-name" match="parameterlist[@kind eq 'exception']">Thrown On</xsl:template>


  <xsl:template mode="table-body" match="parameterlist">
    <xsl:apply-templates mode="parameter-row" select="parameteritem"/>
  </xsl:template>

          <xsl:template mode="parameter-row" match="parameteritem">
            <tr>
              <td>
                <code>
                  <!-- ASSUMPTION: <parameternamelist> only ever has one <parametername> child -->
                  <xsl:apply-templates select="parameternamelist/parametername/node()"/>
                </code>
              </td>
              <td>
                <xsl:apply-templates select="parameterdescription/node()"/>
              </td>
            </tr>
          </xsl:template>

  <xsl:template mode="table-body" match="sectiondef[@kind eq 'enum']">
    <xsl:apply-templates mode="enum-row" select="memberdef/enumvalue"/> <!-- Use input order for enum values -->
  </xsl:template>

          <xsl:template mode="enum-row" match="enumvalue">
            <tr>
              <td>
                <code>{name}</code>
              </td>
              <td>
                <xsl:apply-templates select="briefdescription, detaileddescription"/>
              </td>
            </tr>
          </xsl:template>

  <xsl:template mode="table-body" match="sectiondef | innerclass">
    <xsl:variable name="member-nodes" as="element()*">
      <xsl:apply-templates mode="member-nodes" select="."/>
    </xsl:variable>
    <xsl:for-each-group select="$member-nodes" group-by="d:member-name(.)">
      <xsl:sort select="current-grouping-key()"/>
      <xsl:apply-templates mode="member-row" select="."/>
    </xsl:for-each-group>
  </xsl:template>

          <xsl:template mode="member-nodes" match="innerclass | sectiondef[@kind eq 'public-type']">
            <xsl:param name="public-types" tunnel="yes" select="()"/>
            <xsl:sequence select="$public-types"/>
          </xsl:template>

          <xsl:template mode="member-nodes" match="sectiondef[@kind eq 'friend']">
            <xsl:param name="friends" tunnel="yes"/>
            <xsl:sequence select="$friends"/>
          </xsl:template>

          <xsl:template mode="member-nodes" match="sectiondef">
            <!--
              ASSUMPTION (for now): At least one member per section (table) must not be in a user-defined group.
              Also, we may need a more robust mapping between a user-defined group's members and the sections
              in which they belong. For now, we are using this partial test.
            -->
            <xsl:sequence select="memberdef,
                                  ../sectiondef[@kind eq 'user-defined']/memberdef[(@kind||@prot||@static) =
                                                               current()/memberdef/(@kind||@prot||@static)]"/>
          </xsl:template>


          <xsl:function name="d:member-name">
            <xsl:param name="element"/>
            <xsl:apply-templates mode="member-name" select="$element"/>
          </xsl:function>

                  <xsl:template mode="member-name" match="memberdef">
                    <xsl:sequence select="name"/>
                  </xsl:template>
                  <xsl:template mode="member-name" match="innerclass">
                    <xsl:sequence select="d:referenced-inner-class/compounddef/compoundname ! d:strip-ns(.)"/>
                  </xsl:template>


          <xsl:template mode="member-row" match="*">
            <tr>
              <td>
                <bold>
                  <ref d:refid="{@d:page-refid}">{current-grouping-key()}</ref>
                  <xsl:apply-templates mode="member-annotation" select="."/>
                </bold>
              </td>
              <td>
                <xsl:apply-templates mode="member-description" select="."/>
              </td>
            </tr>
          </xsl:template>

                  <xsl:template mode="member-annotation" match="*">
                    <xsl:variable name="member-name" select="current-grouping-key()"/>
                    <xsl:variable name="is-destructor" select="starts-with($member-name, '~')"/>
                    <xsl:variable name="is-constructor" select="$member-name = d:strip-ns(/doxygen/compounddef/compoundname)"/>
                    <xsl:if test="$is-destructor or $is-constructor">
                      <xsl:text>&#160;</xsl:text>
                      <role class="silver">
                        <xsl:choose>
                          <xsl:when test="$is-destructor">[destructor]</xsl:when>
                          <xsl:otherwise                 >[constructor]</xsl:otherwise>
                        </xsl:choose>
                      </role>
                    </xsl:if>
                  </xsl:template>


                  <xsl:template mode="member-description" match="innerclass">
                    <xsl:apply-templates select="d:referenced-inner-class/compounddef/briefdescription"/>
                  </xsl:template>
                  <xsl:template mode="member-description" match="memberdef">
                    <xsl:variable name="descriptions" select="current-group()/briefdescription"/>
                    <!-- Pull in any overload descriptions but only if they vary -->
                    <xsl:for-each select="distinct-values($descriptions)">
                      <!-- ASSUMPTION: <briefdescription> always contains one <para> -->
                      <xsl:apply-templates select="$descriptions[. eq current()][1]/para/node()"/>
                      <xsl:if test="position() ne last()">
                        <br/>
                        <role class="silver">—</role>
                        <br/>
                      </xsl:if>
                    </xsl:for-each>
                  </xsl:template>


  <xsl:template mode="section-body" match="detaileddescription | simplesect">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template mode="section-body" match="compounddef | memberdef" priority="1">
    <xsl:apply-templates mode="includes" select="."/>
    <xsl:next-match/>
  </xsl:template>

  <xsl:template mode="section-body" match="compounddef">
    <compound>
      <xsl:apply-templates mode="normalize-params" select="templateparamlist"/>
      <kind>{@kind}</kind>
      <name>{d:strip-ns(compoundname)}</name>
      <xsl:for-each select="basecompoundref[not(d:should-ignore-base(.))]
                                           [not(@prot eq 'private')]">
        <base>
          <prot>{@prot}</prot>
          <name>{d:strip-doc-ns(.)}</name>
        </base>
      </xsl:for-each>
    </compound>
  </xsl:template>

  <xsl:template mode="section-body" match="memberdef[@kind eq 'typedef']">
    <typedef>
      <xsl:apply-templates mode="normalize-params" select="templateparamlist"/>
      <xsl:apply-templates select="name, type"/>
    </typedef>
    <!-- Nested compounddefs were derived from refs in the typedef's type -->
    <xsl:apply-templates select="d:referenced-typedef-class/compounddef"/>
  </xsl:template>

      <!-- Suppress the display of several aspects of referenced typedef classes -->

          <!-- Don't show the briefdescription -->
          <xsl:template match="d:referenced-typedef-class/compounddef/briefdescription"/>

          <!-- Don't show the Synopsis -->
          <xsl:template mode="section" match="d:referenced-typedef-class/compounddef"/>

          <!-- Exclude the "Description" heading (only show the body) -->
          <xsl:template mode="section" match="d:referenced-typedef-class/compounddef/detaileddescription">
            <xsl:apply-templates mode="section-body" select="."/>
          </xsl:template>

          <!-- Don't show the includes header or footer -->
          <xsl:template mode="includes" match="d:referenced-typedef-class/compounddef"/>


  <xsl:template mode="section-body" match="memberdef[@kind eq 'enum']">
    <enum>
      <xsl:apply-templates select="name"/>
    </enum>
  </xsl:template>

  <xsl:template mode="section-body" match="memberdef[@kind eq 'variable']">
    <variable>
      <xsl:apply-templates mode="modifier" select="@static[. eq 'yes']"/>
      <xsl:apply-templates select="type, name"/>
      <xsl:apply-templates select="initializer[count(../initializer) eq 1]"/>
    </variable>
  </xsl:template>

  <xsl:template mode="section-body" match="memberdef[@kind = ('function','friend')]">
    <function>
      <xsl:apply-templates mode="normalize-params" select="templateparamlist"/>
      <xsl:apply-templates mode="modifier" select="@static[. eq 'yes'],
                                                   @virt  [. eq 'virtual']"/>
      <xsl:apply-templates select="type, name"/>
      <params>
        <xsl:apply-templates select="param"/>
      </params>
      <xsl:apply-templates mode="modifier" select="@const[. eq 'yes']"/>
    </function>
  </xsl:template>

          <!-- Extract <declname> when Doxygen hides it in the <type> -->
          <xsl:template mode="normalize-params" match="templateparamlist/param/type[not(../declname)]
                                                                                   [starts-with(.,'class ') or
                                                                                    starts-with(.,'typename ')]"
                                                priority="1">
            <type>{substring-before(.,' ')}</type>
            <declname>{substring-after(.,' ')}</declname>
          </xsl:template>

          <!-- Flag as error if no declname value could be found (unless the type is simply "class") -->
          <xsl:template mode="normalize-params" match="templateparamlist/param/type[not(../declname)]
                                                                                   [not(. = 'class')]">
            <ERROR message="param neither has a declname nor a 'class ' or 'typename ' prefix in the type"/>
          </xsl:template>

          <xsl:template mode="normalize-params" match="templateparamlist/param/defname"/>


  <!-- We only need to keep the @file attribute -->
  <xsl:template match="location/@*[. except ../@file]"/>

  <xsl:template match="briefdescription | detaileddescription">
    <div>
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <xsl:template match="simplesect/title"/>

  <!-- TODO: verify we don't need this; it was causing duplicate headings in simplesect sections
  <xsl:template match="title">
    <heading>
      <xsl:apply-templates/>
    </heading>
  </xsl:template>
  -->

  <!-- By default, don't output an includes header or footer -->
  <xsl:template mode="includes" match="*"/>

  <!-- TODO: Review; this is meant to effect what the previous code did, but I'm not sure it captures the original intentions -->
  <xsl:template mode="includes" match="compounddef
                                     | memberdef[@kind eq 'friend' or ../../@kind eq 'namespace']
                                                [not(/doxygen/@d:page-type eq 'overload-list')]">
    <xsl:param name="is-footer"/>
    <para>
      <xsl:choose>
        <xsl:when test="$is-footer">
          <footer>
            <xsl:apply-templates select="location"/>
          </footer>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select="location"/>
        </xsl:otherwise>
      </xsl:choose>
    </para>
  </xsl:template>

  <!-- Strip the project namespace prefix from link display names
       (matching the text node so this will still work with the strip-leading-space mode/process below) -->
  <!-- TODO: figure out if we need anything like this (maybe not).
  <xsl:template match="ref/text()[starts-with(.,$doc-ns)]">
    <xsl:value-of select="d:strip-doc-ns(.)"/>
  </xsl:template>
  -->


  <!-- When a <para> directly contains a mix of inline nodes and block-level elements, normalize its content -->
  <xsl:template match="para[&BLOCK_LEVEL_ELEMENT;]">
    <para>
      <xsl:for-each-group select="* | text()" group-adjacent="d:is-inline(.)">
        <xsl:apply-templates mode="capture-ranges" select="."/>
      </xsl:for-each-group>
    </para>
  </xsl:template>

          <xsl:function name="d:is-inline">
            <xsl:param name="node"/>
            <xsl:sequence select="not($node/../(&BLOCK_LEVEL_ELEMENT;)[. is $node])"/>
          </xsl:function>

          <!-- Process the block-level elements as usual -->
          <xsl:template mode="capture-ranges" match="node()">
            <xsl:apply-templates select="current-group()"/>
          </xsl:template>

          <!-- Wrap contiguous ranges of inline children in a nested <para> -->
          <xsl:template mode="capture-ranges" match="node()[d:is-inline(.)]">
            <xsl:choose>
              <!-- But only if it has text or if the group has more than one node -->
              <xsl:when test="normalize-space(.) or current-group()[2]">
                <para>
                  <xsl:apply-templates mode="strip-leading-space" select="."/>
                </para>
              </xsl:when>
              <xsl:otherwise>
                <xsl:next-match/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:template>


  <!-- Strip leading whitespace from the nested paragraphs to prevent eventual interpretation as a code block -->
  <xsl:template mode="strip-leading-space" match="*">
    <xsl:copy>
      <xsl:apply-templates mode="#current" select="@* | node()[1]"/>
    </xsl:copy>
    <xsl:apply-templates mode="#current" select="following-sibling::node()[1]
                                                 [ancestor-or-self::node() intersect current-group()]"/>
  </xsl:template>

  <xsl:template mode="strip-leading-space" match="@*">
    <xsl:copy/>
  </xsl:template>

  <xsl:template mode="strip-leading-space" match="text()">
    <xsl:param name="done-stripping" tunnel="yes" select="false()"/>
    <xsl:choose>
      <xsl:when test="$done-stripping">
        <xsl:apply-templates select="."/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:sequence select="replace(.,'^\s+','')"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:apply-templates mode="#current" select="following-sibling::node()[1]
                                                 [ancestor-or-self::node() intersect current-group()]">
      <xsl:with-param name="done-stripping" select="$done-stripping or normalize-space(.)" tunnel="yes"/>
    </xsl:apply-templates>
  </xsl:template>


  <xsl:template mode="#default normalize-params" match="@* | node()">
    <xsl:copy copy-namespaces="no">
      <xsl:apply-templates mode="#current" select="@* | node()"/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
