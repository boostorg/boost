<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml" encoding="utf-8"/>

  <xsl:template match="/">
    <root>
      <expected-failures>
        <xsl:apply-templates select="*/test-log"/>
      </expected-failures>
    </root>
  </xsl:template>
  
  <xsl:template match="test-log">
    <test-result library="{@library}" test-name="{@test-name}" toolset="{@toolset}" result="{@result}" />
  </xsl:template>

</xsl:stylesheet>
