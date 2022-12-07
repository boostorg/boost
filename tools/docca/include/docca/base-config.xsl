<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  exclude-result-prefixes="xs d"
  expand-text="yes">

  <!-- Projects should override these default values using a project-specific custom-overrides.xsl module -->
  <xsl:variable name="doc-ref" select="'docca.ref'"/>
  <xsl:variable name="doc-ns" select="'example'"/>

  <xsl:variable name="include-private-members" select="false()"/>

  <xsl:variable name="additional-id-replacements" as="element(replace)*">
    <replace pattern="boost::asio::error" with=""/>
  </xsl:variable>

  <xsl:variable name="additional-type-replacements" as="element(replace)*">
    <replace pattern="BOOST_ASIO_DECL ?(.*)" with="$1"/>
  </xsl:variable>

  <!-- TODO: refactor the stage-two-specific rules into a separate module that can't intefere with stage one -->
  <xsl:template mode="includes-template" match="location"
    >Defined in header [include_file {substring-after(@file, 'include/')}]
  </xsl:template>

  <xsl:function name="d:should-ignore-compound">
    <xsl:param name="element" as="element(compound)"/>
    <xsl:sequence select="contains($element/name, '::detail')"/>  <!-- TODO: Confirm this should be custom and not built-in behavior -->
  </xsl:function>

  <xsl:function name="d:should-ignore-base">
    <xsl:param name="element" as="element(basecompoundref)"/>
    <xsl:sequence select="contains($element, '::detail')"/>  <!-- TODO: Confirm this should be custom and not built-in behavior -->
  </xsl:function>

  <xsl:function name="d:should-ignore-inner-class">
    <xsl:param name="element" as="element(innerclass)"/>
    <xsl:sequence select="contains($element, '_handler')"/>
  </xsl:function>

  <xsl:function name="d:should-ignore-friend">
    <xsl:param name="element" as="element(memberdef)"/>
    <xsl:sequence select="contains($element, '_helper')"/>
  </xsl:function>

</xsl:stylesheet>
