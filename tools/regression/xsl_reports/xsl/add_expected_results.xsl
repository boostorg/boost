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
  <xsl:param name="failures_markup_file"/>
  <xsl:variable name="expected_results" select="document( $expected_results_file )" />
  <xsl:variable name="failures_markup" select="document( $failures_markup_file )" />

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

      <xsl:variable name="expected_results_test_case" select="$expected_results//*/test-result[ @library=$library and ( @test-name=$test-name or @test-name='*' ) and @toolset = $toolset]"/>
      <xsl:variable name="failures_markup" select="$failures_markup//*/mark[ @library=$library and ( @test = $test-name or @test = '*' ) and ( @toolset = $toolset or @toolset = '*' ) ]"/>

      <xsl:variable name="is_new">
         <xsl:choose>
            <xsl:when test="$expected_results_test_case">
               <xsl:text>no</xsl:text>
            </xsl:when>
            <xsl:otherwise>yes</xsl:otherwise>
         </xsl:choose>
      </xsl:variable>


      <xsl:variable name="expected_result">
        <xsl:choose>
          <xsl:when test='count( $failures_markup ) &gt; 0'>
            <xsl:text>fail</xsl:text>
          </xsl:when>
          
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$expected_results_test_case and $expected_results_test_case/@result = 'fail'">
                <xsl:text>fail</xsl:text>
              </xsl:when>
              
              <xsl:otherwise>success</xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <!--      <a><xsl:value-of select="count( $failures_markup )"/></a> -->
      <xsl:variable name="status">
        <xsl:choose>
          <xsl:when test="count( $failures_markup ) &gt; 0">
            <xsl:choose>
              <xsl:when test="$expected_result = $actual_result">expected</xsl:when>
              <xsl:otherwise>unexpected</xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$expected_result = $actual_result">expected</xsl:when>
              <xsl:otherwise>unexpected</xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
          
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="notes">
        <xsl:choose>
          <xsl:when test='count( $failures_markup ) &gt; 0'>
            <xsl:for-each select="$failures_markup/note">
              <xsl:copy-of select="."/>
            </xsl:for-each>
            <!--        <xsl:copy-of select="$expected_results_test_case/node()"/>-->
          </xsl:when>

          
        </xsl:choose>
      </xsl:variable>

      <xsl:attribute name="result"><xsl:value-of select="$actual_result"/></xsl:attribute>
      <xsl:attribute name="expected-result"><xsl:value-of select="$expected_result"/></xsl:attribute>
      <xsl:attribute name="status"><xsl:value-of select="$status"/></xsl:attribute>
      <xsl:attribute name="is-new"><xsl:value-of select="$is_new"/></xsl:attribute>
      <xsl:element name="notes"><xsl:copy-of select="$notes"/></xsl:element>


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
