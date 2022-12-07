<!-- Concatenate all the text files from a directory into one big file -->
<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  exclude-result-prefixes="xs">

  <xsl:output method="text"/>

  <!-- TODO: remove this coupling; should be configured in the build file somehow
             (add support for Saxon parameter-passing in the build?) -->
  <xsl:param name="input-dir" select="'stage2/results'"/>

  <xsl:template match="page">
    <xsl:copy-of select="unparsed-text(concat($input-dir, '/', @href))"/>
    <xsl:apply-templates select="page"/>
    <!-- Two line breaks before to ensure it doesn't get absorbed into a preceding list -->
    <xsl:text>&#xA;&#xA;[endsect]&#xA;</xsl:text>
  </xsl:template>

</xsl:stylesheet>
