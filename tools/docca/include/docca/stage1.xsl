<xsl:stylesheet version="3.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  exclude-result-prefixes="xs"
  expand-text="yes">

  <xsl:import href="base-stage1.xsl"/>
  <xsl:import href="base-config.xsl"/>

  <!-- Custom config module copied from project-specific directory -->
  <xsl:include href="custom-overrides.xsl"/>

</xsl:stylesheet>
