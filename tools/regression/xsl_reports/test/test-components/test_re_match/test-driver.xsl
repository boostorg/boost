<?xml version="1.0" encoding="utf-8"?>
<!--

Copyright MetaCommunications, Inc. 2003-2004.

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)

-->

<xsl:stylesheet 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:exsl="http://exslt.org/common"
    xmlns:func="http://exslt.org/functions"
    xmlns:str="http://exslt.org/strings"
    xmlns:meta="http://www.meta-comm.com"
    extension-element-prefixes="func"
    exclude-result-prefixes="str meta exsl"
    version="1.0">

    <xsl:import href="../../../xsl/v2/common.xsl"/>

    <xsl:template match="/">
        <xsl:apply-templates />
    </xsl:template>
    <xsl:template match='test'>
        <xsl:variable name="result" select="meta:re_match( @pattern, @text )"/>
        <xsl:variable name="expected-result" select="@result = 'true'"/>
        <result regex="{@pattern}" text="{@text}" result="{$result}"/>
    </xsl:template>

</xsl:stylesheet>
