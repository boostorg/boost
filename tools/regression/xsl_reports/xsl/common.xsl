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
  xmlns:func="http://exslt.org/functions"
  xmlns:meta="http://www.meta-comm.com"
  extension-element-prefixes="func"
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

  <func:function name="meta:is_unusable">
      <xsl:param name="explicit_markup"/>
      <xsl:param name="library"/>
      <xsl:param name="toolset"/>
      
      <func:result select="$explicit_markup//library[ @name = $library ]/mark-unusable[ toolset/@name = $toolset or toolset/@name='*' ]"/>
  </func:function>

  <xsl:template name="show_notes">
      <xsl:param name="explicit_markup"/>
      <xsl:param name="notes"/>
        <div class="notes">
          <xsl:for-each select="$notes">
            <div>
                <xsl:variable name="refid" select="@refid"/>
                <xsl:call-template name="show_note">
                    <xsl:with-param name="note" select="."/>
                    <xsl:with-param name="reference" select="$explicit_markup//note[ $refid = @id ]"/>
                </xsl:call-template>
            </div>
          </xsl:for-each>
        </div>
  </xsl:template>

  <xsl:template name="show_note">
      <xsl:param name="note"/>
      <xsl:param name="reference"/>
      <div class="note">
          <xsl:variable name="author">
              <xsl:choose>
                  <xsl:when test="$note/@author">
                      <xsl:value-of select="$note/@author"/>
                  </xsl:when>
                  <xsl:when test="$reference">
                      <xsl:value-of select="$reference/@author"/>                               
                  </xsl:when>
                  <xsl:otherwise>
                      <xsl:text/>
                  </xsl:otherwise>
              </xsl:choose>
          </xsl:variable>

          <xsl:variable name="date">
              <xsl:choose>
                  <xsl:when test="$note/@date">
                      <xsl:value-of select="$note/@date"/>
                  </xsl:when>
                  <xsl:when test="$reference">
                      <xsl:value-of select="$reference/@date"/>                      
                  </xsl:when>
                  <xsl:otherwise>
                      <xsl:text/>
                  </xsl:otherwise>
              </xsl:choose>
          </xsl:variable>

      <span class="note-header">
          <xsl:choose>
              <xsl:when test="$author != '' and $date != ''">
                  [&#160;<xsl:value-of select="$author"/>&#160;<xsl:value-of select="$date"/>&#160;]
              </xsl:when>
              <xsl:when test="$author != ''">
                  [&#160;<xsl:value-of select="$author"/>&#160;]                        
              </xsl:when>
              <xsl:when test="$date != ''">
                  [&#160;<xsl:value-of select="$date"/>&#160;]                        
              </xsl:when>
          </xsl:choose>
      </span>

      <xsl:if test="$reference">
          <xsl:copy-of select="$reference/node()"/>                
      </xsl:if>
      <xsl:copy-of select="$note/node()"/>      

      </div>
  </xsl:template>


</xsl:stylesheet>
