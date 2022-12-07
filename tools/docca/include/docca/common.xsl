<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  exclude-result-prefixes="xs d">

  <xsl:variable name="nl" select="'&#xA;'"/>

  <xsl:variable name="leading-ns-regex" select="'^([^:&lt;]+::)+'"/>

  <xsl:function name="d:extract-ns-without-suffix">
    <xsl:param name="name"/>
    <xsl:sequence select="replace(d:extract-ns($name), '::$', '')"/>
  </xsl:function>

  <xsl:function name="d:extract-ns">
    <xsl:param name="name"/>
    <xsl:sequence select="replace($name, '('||$leading-ns-regex||').*', '$1')"/>
  </xsl:function>

  <!-- Strip all C++ namespace prefixes that come at the beginning -->
  <xsl:function name="d:strip-ns">
    <xsl:param name="name"/>
    <xsl:sequence select="replace($name, $leading-ns-regex, '')"/>
  </xsl:function>

  <!-- Strip the common C++ namespace prefix for the docs as a whole -->
  <!-- ASSUMPTION: $doc-ns is defined in the customizing stylesheet -->
  <xsl:function name="d:strip-doc-ns">
    <xsl:param name="name"/>
    <xsl:sequence select="if ($name eq $doc-ns)
                          then ''
                          else replace($name, '^'||$doc-ns||'::', '')"/>
  </xsl:function>

  <xsl:function name="d:make-id">
    <xsl:param name="name"/>
    <xsl:sequence select="d:perform-replacements($name, $id-replacements)"/>
  </xsl:function>

  <xsl:function name="d:perform-replacements">
    <xsl:param name="name"/>
    <xsl:param name="replacements"/>
    <xsl:variable name="next" select="head($replacements)"/>
    <xsl:variable name="rest" select="tail($replacements)"/>
    <xsl:sequence select="if (exists($next))
                          then d:perform-replacements(replace($name, $next/@pattern, $next/@with), $rest)
                          else $name"/>
  </xsl:function>

  <xsl:variable name="id-replacements" select="$additional-id-replacements, $base-id-replacements"/>

  <!-- Can be overridden by a customizing stylesheet -->
  <xsl:variable name="additional-id-replacements" as="element(replace)*" select="()"/>

  <xsl:variable name="base-id-replacements" as="element(replace)+">
    <replace pattern="::"   with="__"/>
    <replace pattern="="    with="_eq_"/>
    <replace pattern="!"    with="_not_"/>
    <replace pattern="->"   with="_arrow_"/>
    <replace pattern="&lt;" with="_lt_"/>
    <replace pattern=">"    with="_gt_"/>
    <replace pattern="~$"   with="_bnot_"/>  <!-- bitwise NOT -->
    <replace pattern="~"    with="_dtor_"/>  <!-- destructor -->
    <replace pattern="\["   with="_lb_"/>
    <replace pattern="\]"   with="_rb_"/>
    <replace pattern="\("   with="_lp_"/>
    <replace pattern="\)"   with="_rp_"/>
    <replace pattern="\+"   with="_plus_"/>
    <replace pattern="-"    with="_minus_"/>
    <replace pattern="\*"   with="_star_"/>
    <replace pattern="/"    with="_slash_"/>
    <replace pattern=" "    with="_"/>
  </xsl:variable>

  <xsl:function name="d:cleanup-param">
    <xsl:param name="name"/>
    <xsl:sequence select="d:perform-replacements($name, $param-replacements)"/>
  </xsl:function>

  <xsl:function name="d:cleanup-type">
    <xsl:param name="name"/>
    <xsl:sequence select="d:perform-replacements($name, $type-replacements)"/>
  </xsl:function>

  <xsl:variable name="param-replacements" select="$additional-param-replacements, $base-param-replacements"/>

  <!-- Can be overridden by a customizing stylesheet -->
  <xsl:variable name="additional-param-replacements" as="element(replace)*" select="()"/>

  <xsl:variable name="base-param-replacements" as="element(replace)*">
    <!-- Reformats '*', '&', and '...' in parameters, e.g. "void const*" -->
    <replace pattern=" \*$" with="*"/>
    <replace pattern=" (&amp;&amp;?)(\.{{3}})?$" with="$1$2"/>
  </xsl:variable>

  <!-- NOTE: $type-replacements includes $param-replacements -->
  <xsl:variable name="type-replacements" select="$additional-type-replacements, $base-type-replacements, $param-replacements"/>

  <!-- Can be overridden by a customizing stylesheet -->
  <xsl:variable name="additional-type-replacements" as="element(replace)*" select="()"/>

  <xsl:variable name="base-type-replacements" as="element(replace)*">
    <replace pattern="^virtual$" with=""/>
  </xsl:variable>

</xsl:stylesheet>
