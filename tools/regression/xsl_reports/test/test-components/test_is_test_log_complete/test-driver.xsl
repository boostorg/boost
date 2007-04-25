<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
    xmlns:func="http://exslt.org/functions"
    xmlns:meta="http://www.meta-comm.com"
    extension-element-prefixes="func"
    exclude-result-prefixes="func meta">

    <xsl:import href="../../../xsl/v2/add_expected_results.xsl"/>
    
    <xsl:template match="/">
        <xsl:apply-templates/>
    </xsl:template>
    
    <xsl:template match="//test-log">
        <xsl:variable name="is_complete" select="meta:is_test_log_complete( . )"/>
        <result name="{@test-name}"  target-directory="{@target-directory}" result="{$is_complete}" />
    </xsl:template> 
    
</xsl:stylesheet>
