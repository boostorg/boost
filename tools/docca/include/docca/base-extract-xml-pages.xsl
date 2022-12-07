<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  exclude-result-prefixes="xs d"
  expand-text="yes">

  <!-- TODO: make sure this doesn't screw up any formatting -->
  <!-- NEW TODO: verify we don't need/want this -->
  <!--
  <xsl:output indent="yes"/>
  -->

  <xsl:include href="common.xsl"/>

  <xsl:key name="visible-memberdefs-by-id"
           match="memberdef[$include-private-members or not(@prot eq 'private')]"
           use="@id"/>

  <xsl:key name="elements-by-refid" match="compound | member" use="@refid"/>

  <xsl:variable name="index-xml" select="/"/>

  <xsl:template match="/">
    <index>
      <xsl:apply-templates select="/doxygenindex/compound"/>
    </index>
    <!-- Testing the ID-related functions
    <xsl:value-of select="replace(d:extract-ns('put'), '::$', '')"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="replace(d:extract-ns('foobar::parser::put'), '::$', '')"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="d:extract-ns('foobar::parser::put&lt;foo::bar, bat::bang>')"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="d:strip-ns('boost::beast::http::parser::basic_parser&lt; foo::isRequest, bar::parser &gt;')"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="d:strip-doc-ns('boost::beast::http::parser::basic_parser&lt; foo::isRequest, bar::parser &gt;')"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="d:make-id('boost::beast::http::parser::basic_parser&lt; foo::isRequest, bar::parser &gt;')"/>
    -->
  </xsl:template>

  <!-- Default implementation; can be customized/overridden -->
  <xsl:function name="d:should-ignore-compound">
    <xsl:param name="compound" as="element(compound)"/>
    <xsl:sequence select="false()"/>
  </xsl:function>

  <xsl:template match="compound[d:should-ignore-compound(.)]"/>
  <xsl:template match="compound">
    <!-- Load each input file only once -->
    <xsl:variable name="source-doc" select="d:get-source-doc(.)"/>
    <!-- Ignore private classes unless private members are enabled -->
    <xsl:if test="$include-private-members or not($source-doc/doxygen/compounddef/@prot eq 'private')">
      <!-- Look up memberdefs (and constrain by visibility) only once -->
      <xsl:variable name="memberdefs" select="key('visible-memberdefs-by-id', member/@refid, $source-doc)"/>
      <!-- Create a filtered copy of members within their minimal context, listing only the visible ones -->
      <xsl:variable name="visible-members" as="element(member)*">
        <xsl:variable name="compound" as="element()">
          <compound kind="{@kind}" refid="{@refid}">
            <name>{name}</name>
            <xsl:copy-of select="member[@refid = $memberdefs/@id]"/>
          </compound>
        </xsl:variable>
        <xsl:sequence select="$compound/member"/>
      </xsl:variable>
      <xsl:apply-templates mode="create-page" select=".">
        <xsl:with-param name="source-doc" select="$source-doc" tunnel="yes"/>
        <xsl:with-param name="memberdefs" select="$memberdefs" tunnel="yes"/>
        <xsl:with-param name="visible-members" select="$visible-members" tunnel="yes"/>
      </xsl:apply-templates>
    </xsl:if>
  </xsl:template>

          <xsl:function name="d:get-source-doc" as="document-node()">
            <xsl:param name="compound" as="element(compound)"/>
            <xsl:sequence select="document($compound/@refid||'.xml', $index-xml)"/>
          </xsl:function>

  <!-- Split up the content into class, struct, and member pages -->
  <xsl:template mode="create-page" match="*"/>
  <xsl:template mode="create-page" match="compound[@kind = 'namespace']">
    <xsl:apply-templates mode="child-pages" select="."/>
  </xsl:template>
  <xsl:template mode="create-page" match="compound[@kind = ('class','struct')]
                                        | compound/member">
    <xsl:variable name="page-id" as="xs:string">
      <xsl:apply-templates mode="page-id" select="."/>
    </xsl:variable>
    <page id="{$page-id}" href="{$page-id}.xml">
      <xsl:result-document href="xml-pages/{$page-id}.xml">
        <xsl:apply-templates mode="page-content" select=".">
          <xsl:with-param name="page-id" select="$page-id" tunnel="yes"/>
        </xsl:apply-templates>
      </xsl:result-document>
      <xsl:apply-templates mode="child-pages" select="."/>
    </page>
  </xsl:template>

          <!-- Create the member page for each child (or, if overloaded, the overload-list page) -->
          <xsl:template mode="child-pages" match="compound">
            <xsl:param name="visible-members" tunnel="yes"/>
            <!-- Create a page for each unique member name -->
            <xsl:for-each select="$visible-members[not(name = preceding-sibling::member/name)]">
              <xsl:apply-templates mode="create-page" select=".">
                <xsl:with-param name="is-overload-list-page" select="d:is-overloaded(.)" tunnel="yes"/>
              </xsl:apply-templates>
            </xsl:for-each>
          </xsl:template>

          <!-- A member page doesn't have children, unless it is an overload-list page -->
          <xsl:template mode="child-pages" match="compound/member">
            <xsl:param name="is-overload-list-page" tunnel="yes"/>
            <xsl:if test="$is-overload-list-page">
              <xsl:apply-templates mode="create-page" select="d:overloaded-members(.)">
                <xsl:with-param name="is-overload-list-page" select="false()" tunnel="yes"/>
              </xsl:apply-templates>
            </xsl:if>
          </xsl:template>


          <xsl:template mode="page-id" match="compound">{d:make-id(name)}</xsl:template>
          <xsl:template mode="page-id" match="member">
            <xsl:param name="is-overload-list-page" tunnel="yes"/>
            <xsl:value-of>
              <xsl:apply-templates mode="base-member-page-id" select="."/>
              <!-- Append the overload-specific suffix, if applicable -->
              <xsl:if test="d:is-overloaded(.) and not($is-overload-list-page)">
                <xsl:value-of select="d:make-id('.overload'||d:overload-position(.))"/>
              </xsl:if>
            </xsl:value-of>
          </xsl:template>

                  <xsl:function name="d:is-overloaded" as="xs:boolean">
                    <xsl:param name="member" as="element(member)"/>
                    <xsl:sequence select="exists(d:overloaded-members($member)[2])"/>
                  </xsl:function>

                  <xsl:function name="d:overload-position" as="xs:integer">
                    <xsl:param name="member" as="element(member)"/>
                    <xsl:sequence select="1 + count($member/preceding-sibling::member[name eq $member/name])"/>
                  </xsl:function>

                  <xsl:function name="d:overloaded-members" as="element(member)+">
                    <xsl:param name="member" as="element(member)"/>
                    <xsl:sequence select="$member/../member[name eq $member/name]"/>
                  </xsl:function>


          <xsl:template mode="base-member-page-id" priority="1"
                                                   match="compound[@kind eq 'namespace']
                                                                  /member">{d:make-id(../name||'::'||name)}</xsl:template>
          <xsl:template mode="base-member-page-id" match="compound/member">{d:make-id(../name||'.' ||name)}</xsl:template>


          <!-- The content for a class or struct is the original source document, pared down some -->
          <xsl:template mode="page-content" match="compound">
            <xsl:param name="source-doc" tunnel="yes"/>
            <xsl:apply-templates mode="compound-page" select="$source-doc"/>
          </xsl:template>

                  <!-- By default, copy everything -->
                  <xsl:template mode="compound-page" match="@* | node()" name="copy-in-compound-page">
                    <xsl:copy>
                      <xsl:apply-templates mode="#current" select="@*"/>
                      <xsl:apply-templates mode="compound-page-insert" select="."/>
                      <xsl:apply-templates mode="#current"/>
                    </xsl:copy>
                  </xsl:template>

                          <!-- By default, don't insert anything -->
                          <xsl:template mode="compound-page-insert" match="*"/>

                  <xsl:template mode="compound-page" match="listofallmembers"/>

                  <xsl:template mode="compound-page" match="memberdef/@*"/>

                  <!-- But directly inside <memberdef>, don't copy anything... -->
                  <xsl:template mode="compound-page" match="memberdef/node()"/>

                  <!-- ...except for <name>, <briefdescription>, and <type> -->
                  <xsl:template mode="compound-page" match="memberdef/name
                                                          | memberdef/briefdescription
                                                          | memberdef/type" priority="1">
                    <xsl:call-template name="copy-in-compound-page"/>
                  </xsl:template>

                  <!-- Insert a reference to each child member's page ID -->
                  <xsl:template mode="compound-page-insert" match="memberdef">
                    <xsl:attribute name="d:page-refid" select="d:make-id(/doxygen/compounddef/compoundname||'.'||name)"/>
                  </xsl:template>

                  <!-- Alternative implementation in case we need to start controlling whitespace more
                  <xsl:template mode="compound-page" match="memberdef">
                    <memberdef>
                      <xsl:text>&#xA;</xsl:text>
                      <xsl:copy-of select="name"/>
                      <xsl:text>&#xA;</xsl:text>
                      <xsl:copy-of select="briefdescription"/>
                    </memberdef>
                  </xsl:template>
                  -->

          <!-- The content for a member page is a subset of the source document -->
          <xsl:template mode="page-content" match="compound/member">
            <xsl:param name="is-overload-list-page" tunnel="yes"/>
            <xsl:choose>
              <xsl:when test="$is-overload-list-page">
                <!-- For the overload list page, include the content for every like-named member -->
                <xsl:apply-templates mode="list-page" select=".">
                  <xsl:with-param name="applicable-members" select="d:overloaded-members(.)" tunnel="yes"/>
                </xsl:apply-templates>
              </xsl:when>
              <xsl:otherwise>
                <!-- Otherwise, this page is just for one implementation (whether overloaded or not) -->
                <xsl:apply-templates mode="member-page" select="."/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:template>

                  <xsl:template mode="list-page member-page" match="member" priority="2">
                    <xsl:param name="applicable-members" as="element(member)+" select="." tunnel="yes"/>
                    <xsl:param name="source-doc" tunnel="yes"/>
                    <xsl:param name="memberdefs" tunnel="yes"/>
                    <xsl:apply-templates mode="#current" select="$source-doc">
                      <xsl:with-param name="target-memberdefs"
                                      select="$memberdefs[@id = $applicable-members/@refid]"
                                      tunnel="yes"/>
                      <xsl:with-param name="member" select="." tunnel="yes"/>
                    </xsl:apply-templates>
                  </xsl:template>

                  <!-- Always copy the name of the parent compound -->
                  <xsl:template mode="list-page member-page" match="compoundname" priority="2">
                    <xsl:copy-of select="."/>
                  </xsl:template>

                  <!-- Otherwise, only copy an element if it's the target member or one of its ancestors -->
                  <xsl:template mode="list-page member-page" match="*" priority="1">
                    <xsl:param name="target-memberdefs" tunnel="yes"/>
                    <xsl:if test=". intersect $target-memberdefs/ancestor-or-self::*">
                      <xsl:next-match/>
                    </xsl:if>
                  </xsl:template>

                  <!-- By default, copy everything -->
                  <xsl:template mode="list-page" match="@* | node()">
                    <xsl:copy>
                      <xsl:apply-templates mode="#current" select="@*"/>
                      <xsl:apply-templates mode="list-page-insert" select="."/>
                      <xsl:apply-templates mode="#current"/>
                    </xsl:copy>
                  </xsl:template>

                          <!-- By default, don't insert anything -->
                          <xsl:template mode="list-page-insert" match="*"/>


                  <!-- By default, copy everything -->
                  <xsl:template mode="member-page
                                      copy-member-content" match="@* | node()">
                    <xsl:copy>
                      <xsl:apply-templates mode="#current" select="@*"/>
                      <xsl:apply-templates mode="member-page-insert" select="."/>
                      <xsl:apply-templates mode="#current"/>
                      <xsl:apply-templates mode="member-page-append" select="."/>
                    </xsl:copy>
                  </xsl:template>

                          <!-- By default, don't insert or append anything -->
                          <xsl:template mode="member-page-insert
                                              member-page-append" match="*"/>

                  <!-- Strip out extraneous whitespace -->
                  <xsl:template mode="list-page member-page" match="compounddef/text() | sectiondef/text()"/>

                  <!-- Switch to an unfiltered copy once we're done filtering out the undesired elements -->
                  <xsl:template mode="list-page member-page" match="memberdef/node()" priority="2">
                    <xsl:apply-templates mode="copy-member-content" select="."/>
                  </xsl:template>

                  <!-- Add the page ID to the top of all page types -->
                  <xsl:template mode="compound-page-insert
                                      member-page-insert
                                      list-page-insert" match="/doxygen" priority="2">
                    <xsl:param name="page-id" tunnel="yes"/>
                    <xsl:attribute name="d:page-id" select="$page-id"/>
                    <xsl:next-match/>
                  </xsl:template>

                  <!-- Also, if applicable, insert the overload position and/or base compound reference of this member -->
                  <xsl:template mode="member-page-insert" match="/doxygen" priority="1">
                    <xsl:param name="member" tunnel="yes"/>
                    <xsl:if test="d:is-overloaded($member)">
                      <xsl:attribute name="d:overload-position" select="d:overload-position($member)"/>
                      <xsl:attribute name="d:overload-size" select="count(d:overloaded-members($member))"/>
                    </xsl:if>
                    <xsl:if test="$member[not(starts-with(@refid, ../@refid))]">
                      <xsl:variable name="base-compound" select="$index-xml/*/compound[starts-with($member/@refid, @refid)]
                                                                                      [not(d:should-ignore-compound(.))]"/>
                      <xsl:apply-templates mode="base-compound-atts" select="$base-compound"/>
                    </xsl:if>
                    <xsl:next-match/>
                  </xsl:template>

                          <xsl:template mode="base-compound-atts" match="compound">
                            <xsl:attribute name="d:base-compound-name" select="d:strip-doc-ns(name)"/>
                            <xsl:attribute name="d:base-compound-refid">
                              <xsl:apply-templates mode="page-id" select="."/>
                            </xsl:attribute>
                          </xsl:template>

                  <!-- Make data available for the typedef tables, if applicable -->
                  <xsl:template mode="member-page-append" match="memberdef[@kind eq 'typedef']
                                                                          [type/ref]
                                                                          [not(contains(type, '*'))]">
                    <xsl:for-each select="type/ref">
                      <d:referenced-typedef-class>
                        <xsl:variable name="compound" select="d:get-target-element(.)[self::compound]"/>
                        <xsl:apply-templates mode="compound-page" select="$compound ! d:get-source-doc(.)/*/compounddef"/>
                      </d:referenced-typedef-class>
                    </xsl:for-each>
                  </xsl:template>

                  <!-- Finally, add the page type -->
                  <xsl:template mode="compound-page-insert" match="/doxygen">
                    <xsl:attribute name="d:page-type" select="'compound'"/>
                  </xsl:template>
                  <xsl:template mode="member-page-insert" match="/doxygen">
                    <xsl:attribute name="d:page-type" select="'member'"/>
                  </xsl:template>
                  <xsl:template mode="list-page-insert" match="/doxygen">
                    <xsl:attribute name="d:page-type" select="'overload-list'"/>
                  </xsl:template>

                  <!-- For overload-list pages, include the page id for each member -->
                  <xsl:template mode="list-page-insert" match="memberdef">
                    <xsl:param name="applicable-members" tunnel="yes"/>
                    <xsl:variable name="this-id" select="@id"/>
                    <xsl:variable name="original-member" select="$applicable-members[@refid eq $this-id]"/>
                    <xsl:attribute name="d:page-refid">
                      <xsl:apply-templates mode="page-id" select="$original-member">
                        <xsl:with-param name="is-overload-list-page" select="false()" tunnel="yes"/>
                      </xsl:apply-templates>
                    </xsl:attribute>
                  </xsl:template>

                  <!-- For public innerclasses, insert the referenced class inline -->
                  <xsl:template mode="compound-page-insert" match="innerclass[@prot eq 'public']">
                    <xsl:attribute name="d:page-refid" select="d:make-id(.)"/>
                    <d:referenced-inner-class>
                      <xsl:variable name="compound" select="d:get-target-element(.)" as="element(compound)"/>
                      <xsl:apply-templates mode="compound-page" select="d:get-source-doc($compound)/*/compounddef"/>
                    </d:referenced-inner-class>
                  </xsl:template>

                  <!-- Resolve the referenced page IDs for later link generation -->
                  <xsl:template mode="compound-page-insert member-page-insert" match="ref">
                    <xsl:attribute name="d:refid">
                      <xsl:apply-templates mode="page-id" select="d:get-target-element(.)">
                        <!-- For inline links to member pages, only link to the base page id (no overloads) -->
                        <xsl:with-param name="is-overload-list-page" select="true()" tunnel="yes"/>
                      </xsl:apply-templates>
                    </xsl:attribute>
                  </xsl:template>

                          <xsl:function name="d:get-target-element" as="element()?"> <!-- to allow for partial builds -->
                          <!--
                          <xsl:function name="d:get-target-element" as="element()">
                          -->
                            <xsl:param name="ref" as="element()"/> <!-- <ref> or <innerclass> or... -->
                            <xsl:apply-templates mode="get-target-element" select="$ref">
                              <xsl:with-param name="ref" select="$ref" tunnel="yes"/>
                            </xsl:apply-templates>
                          </xsl:function>

                          <xsl:template mode="get-target-element" match="*">
                            <xsl:param name="ref" as="element()" tunnel="yes"/>
                            <xsl:variable name="referenced-elements" select="key('elements-by-refid', $ref/@refid, $index-xml)"/>
                            <xsl:variable name="result" as="element()?">
                              <xsl:choose>
                                <!-- Handle the case where the referenced element appears two or more times in index.xml -->
                                <!-- If there's no ambiguity, we're done! -->
                                <xsl:when test="count($referenced-elements) eq 1">
                                  <xsl:apply-templates mode="find-actual-target-element" select="$referenced-elements"/>
                                </xsl:when>
                                <xsl:otherwise>
                                  <!-- Otherwise, see if a namespace in the link text successfully disambiguates -->
                                  <xsl:variable name="qualified-reference" as="element()*">
                                    <xsl:variable name="parent-in-link-text"
                                                  select="if (contains($ref,'::'))
                                                          then d:extract-ns-without-suffix($ref)
                                                          else ''"/>
                                    <xsl:sequence select="$referenced-elements[ends-with(parent::compound/name, '::'||$parent-in-link-text)]"/>
                                  </xsl:variable>
                                  <xsl:choose>
                                    <xsl:when test="count($qualified-reference) eq 1">
                                      <xsl:apply-templates mode="find-actual-target-element" select="$qualified-reference"/>
                                    </xsl:when>
                                    <xsl:otherwise>
                                      <!-- Otherwise, favor the member that's in the same class or namespace as the current page -->
                                      <xsl:variable name="sibling-reference" as="element()*">
                                        <xsl:variable name="compound-for-current-page" select="root($ref)/doxygen/compounddef/compoundname/string()"/>
                                        <xsl:sequence select="$referenced-elements[parent::compound/name eq $compound-for-current-page]"/>
                                      </xsl:variable>
                                      <xsl:choose>
                                        <xsl:when test="count($sibling-reference) eq 1">
                                          <xsl:apply-templates mode="find-actual-target-element" select="$sibling-reference"/>
                                        </xsl:when>
                                        <!-- If all else fails, give up and just use the first one -->
                                        <xsl:otherwise>
                                          <xsl:apply-templates mode="find-actual-target-element" select="$referenced-elements[1]"/>
                                        </xsl:otherwise>
                                      </xsl:choose>
                                    </xsl:otherwise>
                                  </xsl:choose>
                                </xsl:otherwise>
                              </xsl:choose>
                            </xsl:variable>
                            <xsl:if test="not($result)">
                              <xsl:message>Unable to find referenced ID: <xsl:value-of select="$ref/@refid"/></xsl:message>
                            </xsl:if>
                            <xsl:sequence select="$result"/>
                          </xsl:template>

                                  <xsl:template mode="find-actual-target-element" match="compound | member">
                                    <xsl:sequence select="."/>
                                  </xsl:template>

                                  <!-- In the index XML, enumvalue "members" immediately follow the corresponding enum member -->
                                  <xsl:template mode="find-actual-target-element" match="member[@kind eq 'enumvalue']">
                                    <xsl:sequence select="d:enum-member(.)"/>
                                  </xsl:template>

                                          <xsl:function name="d:enum-member">
                                            <xsl:param name="member"/>
                                            <xsl:sequence select="$member/preceding-sibling::member[@kind eq 'enum'][1]"/>
                                          </xsl:function>

                                  <!-- Doxygen sometimes confuses refids of like values of enums in the same namespace;
                                       based on a narrow heuristic, find the target it should have assigned instead -->
                                  <xsl:template mode="find-actual-target-element"
                                                match="member[@kind eq 'enumvalue']
                                                             [count(../member[@kind eq 'enumvalue']/name[. eq current()/name]) gt 1]"
                                                priority="1">
                                    <xsl:param name="ref" tunnel="yes"/>
                                    <xsl:variable name="prefix-in-link-text" select="d:extract-ns-without-suffix($ref)"/>
                                    <xsl:choose>
                                      <xsl:when test="not($prefix-in-link-text)">
                                        <!-- If there's no namespace in the link text, trust the refid that Doxygen assigned -->
                                        <xsl:next-match/>
                                      </xsl:when>
                                      <xsl:otherwise>
                                        <!-- Otherwise, see if a different target is a better match -->
                                        <xsl:variable name="repeated-enum-values"
                                                      select="../member[@kind eq 'enumvalue'][name eq current()/name]"/>
                                        <xsl:variable name="candidate-enums"
                                                      select="$repeated-enum-values/d:enum-member(.)"/>
                                        <xsl:variable name="matching-enums"
                                                      select="$candidate-enums[name eq $prefix-in-link-text]"/>
                                        <xsl:choose>
                                          <xsl:when test="count($matching-enums) eq 1">
                                            <!-- If we found exactly one match, use it instead of what Doxygen said -->
                                            <xsl:sequence select="$matching-enums"/>
                                          </xsl:when>
                                          <xsl:otherwise>
                                            <!-- Otherwise, don't second-guess the refid Doxygen assigned -->
                                            <xsl:next-match/>
                                          </xsl:otherwise>
                                        </xsl:choose>
                                      </xsl:otherwise>
                                    </xsl:choose>
                                  </xsl:template>

</xsl:stylesheet>
