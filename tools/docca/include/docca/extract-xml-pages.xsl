<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:d="http://github.com/vinniefalco/docca"
  exclude-result-prefixes="xs d">

  <xsl:import href="base-extract-xml-pages.xsl"/>
  <xsl:import href="base-config.xsl"/>

  <!-- Custom config module copied from project-specific directory -->
  <xsl:include href="custom-overrides.xsl"/>

</xsl:stylesheet>
