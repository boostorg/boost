<?xml version="1.0" encoding="utf-8"?>
<!--

 (C) Copyright MetaCommunications, Inc. 2003.

 Permission to use, copy, modify, distribute and sell this software
 and its documentation for any purpose is hereby granted without fee, 
 provided that the above copyright notice appears in all copies and 
 that both the copyright notice and this permission notice appear in 
 supporting documentation. No representations are made about the 
 suitability of this software for any purpose. It is provided "as is" 
 without express or implied warranty.

-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:exsl="http://exslt.org/common"
  version="1.0">

  <xsl:template name="get_toolsets">
    <xsl:param name="toolsets"/>
    <xsl:param name="required-toolsets"/>
    
    <xsl:variable name="toolset_output">
      <xsl:for-each select="$toolsets">
        <xsl:variable name="toolset" select="."/>
        <xsl:element name="toolset">
          <xsl:attribute name="toolset"><xsl:value-of select="$toolset"/></xsl:attribute>
          <xsl:choose>
            <xsl:when test="$required_toolsets[ $toolset = @name ]">
              <xsl:attribute name="required">yes</xsl:attribute>
              <xsl:attribute name="sort">a</xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="required">no</xsl:attribute>
              <xsl:attribute name="sort">z</xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:element>
      </xsl:for-each>
    </xsl:variable>
    
    <xsl:for-each select="exsl:node-set( $toolset_output )/toolset">
      <xsl:sort select="concat( @sort, ' ', @toolset)" order="ascending"/>
      <xsl:copy-of select="."/>
    </xsl:for-each>
    
  </xsl:template>



</xsl:stylesheet>
