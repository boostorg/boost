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

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
  <xsl:output method="xml" encoding="utf-8"/>
  
  <xsl:param name="expected_results_file"/>
  <xsl:variable name="expected_results" select="document( $expected_results_file )" />

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>
  
  <xsl:template match="test-log">
    <xsl:variable name="library" select="@library"/>
    <xsl:variable name="test-name" select="@test-name"/>
    <xsl:variable name="toolset" select="@toolset"/>

    <xsl:element name="{local-name()}">
      <xsl:apply-templates select="@*"/>

      <xsl:variable name="actual_result">
        <xsl:choose>
          <xsl:when test="./*/@result = 'fail'" >
            <xsl:text>fail</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>success</xsl:text>
          </xsl:otherwise>
        </xsl:choose>                     
      </xsl:variable>

      <xsl:variable name="expected_results_test_case" select="$expected_results//*/test-result[ @library=$library and @test-name=$test-name and @toolset = $toolset]"/>

      <xsl:variable name="expected_result">
        <xsl:choose>
          <xsl:when test="$expected_results_test_case">
            <xsl:text>fail</xsl:text>
          </xsl:when>
          <xsl:otherwise>success</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="status">
        <xsl:choose>
          <xsl:when test="$expected_result = $actual_result">expected</xsl:when>
          <xsl:otherwise>unexpected</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="note">
        <xsl:copy-of select="$expected_results_test_case/node()"/>
      </xsl:variable>

      <xsl:attribute name="result"><xsl:value-of select="$actual_result"/></xsl:attribute>
      <xsl:attribute name="expected-result"><xsl:value-of select="$expected_result"/></xsl:attribute>
      <xsl:attribute name="status"><xsl:value-of select="$status"/></xsl:attribute>
      <xsl:element name="note"><xsl:copy-of select="$note"/></xsl:element>
      <xsl:apply-templates select="node()" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="*">
    <xsl:element name="{local-name()}">
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="@*">
    <xsl:copy-of select="." />
  </xsl:template>

</xsl:stylesheet>
