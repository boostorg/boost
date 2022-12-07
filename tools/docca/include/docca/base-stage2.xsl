<!DOCTYPE xsl:stylesheet [
<!ENTITY SYNTAX_BLOCK "*[ self::compound
                        | self::function
                        | self::typedef
                        | self::enum
                        | self::variable
                        | self::overloaded-member
                        ]">
<!ENTITY CODE_BLOCK "*[ self::computeroutput[not(ref)]
                      | self::code
                      ]">
]>
<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  expand-text="yes">

  <xsl:import href="common.xsl"/>

  <xsl:output method="text"/>

  <xsl:param name="DEBUG" select="false()"/>

  <!-- Can be overridden in library-specific configuration -->
  <xsl:variable name="emphasized-template-parameter-types" select="()"/>

  <xsl:variable name="list-indent-width" select="4"/>

  <xsl:template mode="before" match="/page">
    <xsl:text>{$nl}</xsl:text>
    <xsl:text>[section:{tokenize(@id,'\.')[last()]} {d:qb-escape(title)}]</xsl:text>
    <xsl:apply-templates mode="indexterm" select="."/>
  </xsl:template>

          <xsl:template mode="indexterm" match="page"/>
          <xsl:template mode="indexterm" match="page[@primary-index-term]"
            >{$nl}[indexterm1 {d:qb-escape(@primary-index-term)}]{$nl}</xsl:template>
          <xsl:template mode="indexterm" match="page[@secondary-index-term]" priority="1"
            >{$nl}[indexterm2 {d:qb-escape(@primary-index-term)}..{
                               d:qb-escape(@secondary-index-term)}]{$nl}</xsl:template>

  <!-- Title is already included in section header -->
  <xsl:template match="/page/title"/>

  <xsl:template match="heading">{$nl}[heading {.}]</xsl:template>

  <!-- Sections inside tables don't render well; just display the heading text inline (e.g. "See Also") -->
  <xsl:template match="td//heading">{$nl}{.} </xsl:template>

  <xsl:template match="location">
    <xsl:apply-templates mode="includes-template" select="."/>
  </xsl:template>

  <xsl:template match="footer/location">
    <xsl:apply-templates mode="includes-template-footer" select="."/>
  </xsl:template>

  <xsl:template mode="before" match="&SYNTAX_BLOCK;">{$nl}```{$nl}</xsl:template>
  <xsl:template mode="after"  match="&SYNTAX_BLOCK;">{$nl}```{$nl}</xsl:template>

  <!-- Merge adjacent overloaded-members into one syntax block, separated by one blank line -->
  <xsl:template mode="after"  match="overloaded-member[following-sibling::*[1]/self::overloaded-member]" priority="1"/>
  <xsl:template mode="before" match="overloaded-member[preceding-sibling::*[1]/self::overloaded-member]" priority="1"
    >{$nl}{$nl}</xsl:template>

  <xsl:template mode="after" match="overloaded-member/type[normalize-space(.)]
                                  |          function/type[normalize-space(.)]">{$nl}</xsl:template>

  <xsl:template mode="append" match="function">;</xsl:template>

  <xsl:template mode="append" match="overloaded-member">
    <xsl:text>;{$nl}</xsl:text>
    <xsl:variable name="more-link" as="element()">
      <emphasis>'''&amp;raquo;''' <ref d:refid="{ref/@d:refid}">more...</ref></emphasis>
    </xsl:variable>
    <xsl:text>  ``</xsl:text>
    <xsl:apply-templates select="$more-link"/>
    <xsl:text>``</xsl:text>
  </xsl:template>

  <xsl:template priority="1"
                match="&SYNTAX_BLOCK;//ref">``[link {$doc-ref}.{@d:refid} {d:qb-escape(.)}]``</xsl:template>
  <xsl:template match="td[1]//ref"           >[link {$doc-ref}.{@d:refid} {d:qb-escape(.)}]</xsl:template>
  <xsl:template match="ref"                  >[link {$doc-ref}.{@d:refid} `{.}`]</xsl:template>

  <xsl:template mode="before" match="&CODE_BLOCK;">`</xsl:template>
  <xsl:template mode="after"  match="&CODE_BLOCK;">`</xsl:template>

  <xsl:template mode="before" match="enum/name">enum </xsl:template>

  <xsl:template mode="before" match="typedef/name">using </xsl:template>
  <xsl:template mode="after"  match="typedef/name"> = </xsl:template>
  <xsl:template mode="after"  match="typedef/type">;</xsl:template>

  <xsl:template match="type[. eq '__implementation_defined__'    ]">``['implementation-defined]``</xsl:template>
  <xsl:template match="type[. eq '__see_below__'                 ]">``['see-below]``</xsl:template>
  <xsl:template match="type[. = ('__deduced__','void_or_deduced')]">``__deduced__``</xsl:template>

  <xsl:template mode="before" match="variable/name | variable/initializer">{' '}</xsl:template>
  <xsl:template mode="append" match="variable">;</xsl:template>

  <xsl:template mode="after" match="compound/kind">{' '}</xsl:template>

  <xsl:template mode="before" match="base[1]"> :{$nl}    </xsl:template>
  <xsl:template mode="before" match="base"     >{$nl}    </xsl:template>
  <xsl:template mode="after"  match="base[position() ne last()]">,</xsl:template>

  <xsl:template mode="after" match="base/prot">{' '}</xsl:template>

  <xsl:template mode="before" match="templateparamlist">template&lt;</xsl:template>
  <xsl:template mode="after"  match="templateparamlist">>{$nl}</xsl:template>

  <xsl:template mode="before" match="param">{$nl}    </xsl:template>
  <xsl:template mode="after"  match="param[position() ne last()]">,</xsl:template>

  <xsl:template mode="after" match="param[declname]/type">{' '}</xsl:template>


  <xsl:template mode="before" match="params">(</xsl:template>
  <xsl:template mode="after"  match="params">)</xsl:template>

  <xsl:template match="templateparamlist/param/declname[. = $emphasized-template-parameter-types]"
    >__{translate(.,'_','')}__</xsl:template>

  <xsl:template mode="before" match="defval"> = </xsl:template>

  <xsl:template mode="before" match="modifier[. eq 'const']">{' '}</xsl:template>
  <xsl:template mode="after"  match="modifier[. eq 'const']"/>

  <xsl:template mode="after"  match="modifier">{$nl}</xsl:template>


  <xsl:template mode="#all" match="ERROR">[role red error.{@message}]</xsl:template>

  <xsl:template mode="before" match="table">{$nl}[table </xsl:template>
  <xsl:template mode="after"  match="table">{$nl}]</xsl:template>

  <!-- ASSUMPTION: table rows have either <th> or <td>, not both -->
  <xsl:template mode="before" match="tr[th] | th">[</xsl:template>
  <xsl:template mode="after"  match="tr[th] | th">]</xsl:template>

  <xsl:template mode="before" match="tr">{$nl}  [</xsl:template>
  <xsl:template mode="after"  match="tr">{$nl}  ]</xsl:template>

  <xsl:template mode="before" match="td">{$nl}    [</xsl:template>
  <xsl:template mode="after"  match="td">{$nl}    ]</xsl:template>

  <xsl:template mode="before" match="bold">[*</xsl:template>
  <xsl:template mode="after"  match="bold">]</xsl:template>

  <xsl:template mode="before" match="emphasis">['</xsl:template>
  <xsl:template mode="after"  match="emphasis">]</xsl:template>

  <xsl:template mode="before" match="role">[role {@class} </xsl:template>
  <xsl:template mode="after"  match="role">]</xsl:template>

  <xsl:template mode="before" match="ulink">[@{@url} </xsl:template>
  <xsl:template mode="after"  match="ulink">]</xsl:template>

  <xsl:template mode="after"  match="itemizedlist | orderedlist">{$nl}</xsl:template>

  <xsl:template match="listitem">
    <xsl:text>{$nl}</xsl:text>
    <xsl:apply-templates mode="list-item-indent" select="."/>
    <xsl:apply-templates mode="list-item-label" select=".."/>
    <xsl:text> </xsl:text>
    <!-- ASSUMPTION: <para> always appears as a child of list items -->
    <xsl:apply-templates select="para/node()"/>
  </xsl:template>

          <!-- TODO: verify this works as expected (find an example of a nested list) -->
          <xsl:template mode="list-item-indent"
                        match="listitem">{ancestor::listitem ! (1 to $list-indent-width) ! ' '}</xsl:template>

          <xsl:template mode="list-item-label" match="itemizedlist">*</xsl:template>
          <xsl:template mode="list-item-label" match="orderedlist" >#</xsl:template>

  <!-- Lists inside a table cell require the use of "explicit list tags" for proper rendering -->
  <xsl:template match="td//itemizedlist
                     | td//orderedlist">
    <xsl:text>[</xsl:text>
    <xsl:apply-templates mode="explicit-list-name" select="."/>
    <xsl:text> </xsl:text>
    <xsl:apply-templates mode="explicit-list-item"/>
    <xsl:text>]</xsl:text>
  </xsl:template>

          <xsl:template mode="explicit-list-name" match="itemizedlist">itemized_list</xsl:template>
          <xsl:template mode="explicit-list-name" match="orderedlist">ordered_list</xsl:template>

          <xsl:template mode="explicit-list-item" match="listitem">
            <xsl:text>[</xsl:text>
            <xsl:apply-templates/>
            <xsl:text>]</xsl:text>
          </xsl:template>

  <xsl:template mode="append" match="/page/div[1]">
    <xsl:if test="$DEBUG">
      <xsl:text>['</xsl:text>
      <xsl:text>[role red \[Page type: [*{/*/@type}]\]] </xsl:text>
      <xsl:text>[role green \[[@../../doc/html/{translate($doc-ref,'.','/')}/{
                                                translate(/page/@id,'.','/')}.html [role green doc_build_html]]\]] </xsl:text>
      <xsl:text>[@../build/xml-pages/{/page/@id}.xml                     [role blue   [*\[doxygen_page_xml\]]]]</xsl:text>
      <xsl:text>[@../build/stage1_visualized/visualized/{/page/@id}.html [role magenta ---stage1_visualized-->]]</xsl:text>
      <xsl:text>[@../build/stage1_visualized/results/{   /page/@id}.xml  [role blue   [*\[docca_page_xml\]]]]</xsl:text>
      <xsl:text>[@../build/stage2_visualized/visualized/{/page/@id}.html [role magenta ---stage2_visualized-->]]</xsl:text>
      <xsl:text>[@../build/stage2_visualized/results/{   /page/@id}.txt  [role blue   [*\[quickbook_result\]]]]</xsl:text>
      <xsl:text>]</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template mode="before" match="para | div">{$nl}</xsl:template>

  <xsl:template match="sp">{' '}</xsl:template>

  <xsl:template match="linebreak">{$nl}{$nl}</xsl:template>

  <xsl:template match="br">[br]</xsl:template>

  <xsl:template mode="before" match="programlisting">{$nl}```{$nl}</xsl:template>
  <xsl:template mode="after"  match="programlisting"     >```{$nl}</xsl:template>

  <xsl:template mode="after" match="codeline">{$nl}</xsl:template>

  <!-- Ignore whitespace-only text nodes -->
  <xsl:template match="text()[not(normalize-space())]" priority="1"/>

  <!-- By default, escape Quickbook markup (square brackets) -->
  <xsl:template match="text()">
    <xsl:sequence select="d:qb-escape(.)"/>
  </xsl:template>

  <!-- But don't escape them in these contexts -->
  <xsl:template match="&SYNTAX_BLOCK;//text()
                     | &CODE_BLOCK;//text()
                     | programlisting//text()">
    <!--
      This implementation (using <xsl:sequence> returning a string, instead of <xsl:value-of>) can
      result in a contiguous sequence of strings, which gets converted to a text node having space
      separators between the strings. This is desirable in some cases and not in others.

      TODO: Tighten the rules so that we explicitly add the spaces where we need them but otherwise
      strip them out (probably by using <xsl:value-of> for the rules matching text nodes).
    -->
    <xsl:sequence select="string(.)"/>
  </xsl:template>

  <!-- Boilerplate default rules for elements -->
  <!-- Convention of this stylesheet is to favor use of just "before" and "after"
       and to utilize "append" (and maybe "insert") only when a distinction is needed -->
  <xsl:template match="*" priority="10">
    <xsl:apply-templates mode="before" select="."/>
    <!-- enable if needed/desired
    <xsl:apply-templates mode="insert" select="."/> -->
    <xsl:next-match/>
    <xsl:apply-templates mode="append" select="."/>
    <xsl:apply-templates mode="after" select="."/>
  </xsl:template>

          <!-- Default before/after/insert/append rules are to do nothing -->
          <xsl:template mode="before" match="*"/>
          <!-- enable if needed/desired
          <xsl:template mode="insert" match="*"/> -->
          <xsl:template mode="append" match="*"/>
          <xsl:template mode="after" match="*"/>


  <xsl:function name="d:qb-escape">
    <xsl:param name="string"/>
    <xsl:sequence select="replace(
                            replace($string, '\[', '\\['),
                            '\]',
                            '\\]'
                          )"/>
  </xsl:function>

</xsl:stylesheet>
