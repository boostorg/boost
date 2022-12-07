<!--
  All this stylesheet does is output a .txt file for each (erroneously-but-unavoidably-named)
  .xml file that actually contains a fragment of Quickbook content. This isn't needed for the
  regular build, but it's useful for viewing in the browser when debugging.
-->
<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  exclude-result-prefixes="xs">

  <xsl:output method="text"/>

  <xsl:param name="input-dir"/>

  <xsl:template match="page">
    <xsl:variable name="input-uri" select="$input-dir||'/'||@href"/>
    <xsl:result-document href="{replace($input-uri, '\.xml$', '.txt')}">
      <xsl:copy-of select="unparsed-text($input-uri)"/>
    </xsl:result-document>
    <xsl:apply-templates select="page"/>
  </xsl:template>

</xsl:stylesheet>
