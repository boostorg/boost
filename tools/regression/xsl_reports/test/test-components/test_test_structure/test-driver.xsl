<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:func="http://exslt.org/functions"
    xmlns:meta="http://www.meta-comm.com"
    extension-element-prefixes="func"
    xmlns:exsl="http://exslt.org/common"

    exclude-result-prefixes="func meta" version="1.0">


    <xsl:import href="../../../xsl/v2/common.xsl"/>
    <xsl:param name="failures_markup_file" select="'explicit-failures-markup.xml'" />
    <xsl:variable name="explicit_markup" select="document( $failures_markup_file )"/>

    <xsl:template match="/">
        <results>
        <xsl:copy-of select="meta:test_structure(   ., 'yes', $explicit_markup )"/>
        <xsl:copy-of select="meta:test_structure( ., 'no', $explicit_markup )"/>
        </results>
    </xsl:template>
    
</xsl:stylesheet>
