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
  xmlns:set="http://exslt.org/sets"
  extension-element-prefixes="func"
  version="1.0">

  <xsl:import href="common.xsl"/>

  <xsl:output method="html" 
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" 
    encoding="utf-8" 
    indent="yes"
    />


  <xsl:param name="links_file"/>
  <xsl:param name="mode"/>
  <xsl:param name="source"/>
  <xsl:param name="run_date"/>
  <xsl:param name="comment_file"/>
  <xsl:param name="expected_results_file"/>
  <xsl:param name="explicit_markup_file"/>

  <!-- the author-specified expected test results -->
  <xsl:variable name="explicit_markup" select="document( $explicit_markup_file )"/>
  <xsl:variable name="expected_results" select="document( $expected_results_file )" />
 
  <!-- necessary indexes -->
  <xsl:key 
    name="test_name_key" 
    match="test-log" 
    use="concat( @library, '@', @test-name )"/>
                  <xsl:key 
                      name="a" 
                      match="." 
                      use="concat( @library, '@', @test-name )"/>

  <xsl:key 
    name="library_key" 
    match="test-log" 
    use="@library"/>
  <xsl:key name="toolset_key" match="test-log" use="@toolset"/>

  <!-- toolsets -->

  <xsl:variable name="required_toolsets" select="$explicit_markup//mark-toolset[ @status='required' ]"/>
  <xsl:variable name="required_toolset_names" select="$explicit_markup//mark-toolset[ @status='required' ]/@name"/>
   <!-- libraries -->
  <xsl:variable name="libraries" select="//test-log[ @library != '' and generate-id(.) = generate-id( key('library_key',@library)[1] )  ]/@library"/>

  <xsl:variable name="unexpected_test_cases" select="//test-log[ @status='unexpected' and @result='fail' and @toolset = $required_toolset_names]"/>

  <func:function name="meta:get_library_tests">
      <xsl:param name="tests"/>
      <xsl:param name="library"/>
      
      <xsl:variable name="a">                  
          <xsl:for-each select="$tests[ @library=$library ]">
              <xsl:sort select="@test-name" order="ascending"/>
              <xsl:copy-of select="."/>
          </xsl:for-each>
      </xsl:variable>
      <func:result select="exsl:node-set( $a )/*"/>
  </func:function>


  <xsl:template match="/">
      <html>
          <head>
              <link rel="stylesheet" type="text/css" href="master.css" title="master" />
              <title>Boost not resolved issues: <xsl:value-of select="$source"/></title>
          </head>
          <body>
              <table>
                  <xsl:for-each select="$libraries">
                      <xsl:sort select="." order="ascending"/>
                      <xsl:variable name="library" select="."/>
                  
                      <xsl:variable name="library_tests" select="meta:get_library_tests( $unexpected_test_cases, $library )"/>
                      <xsl:if test="count( $library_tests ) > 0">
                          <xsl:variable name="library_test_names" select="set:distinct( $library_tests/@test-name )"/>

                          <tr>
                              <td colspan="2">
                                  <div class="library-name"><xsl:value-of select="$library"/></div>
                              </td>
                          </tr>
                          
                          <xsl:for-each select="$library_test_names">
                              <xsl:sort select="." order="ascending"/>
                              <xsl:variable name="test_name" select="."/>
                              
                              <xsl:variable name="unexpected_toolsets" select="$library_tests[ @test-name = $test_name and not (meta:is_unusable( $explicit_markup, $library, @toolset )) ]/@toolset"/>
                              
                              <xsl:if test="count( $unexpected_toolsets ) > 0">
                                  <tr>
                                      <td class="test-name">
                                          <xsl:value-of select="$test_name"/>:
                                      </td>
                                      <td>
                                          <xsl:for-each select="$unexpected_toolsets">
                                              <xsl:sort select="." order="ascending"/>
                                              <xsl:if test="position() > 1">,&#160;</xsl:if>
                                              <xsl:value-of select="."/>
                                          </xsl:for-each>
                                      </td>
                                  </tr>
                                  </xsl:if>
                              </xsl:for-each>
                          </xsl:if>
                      </xsl:for-each>
              </table>
          </body>
      </html>
  </xsl:template>
</xsl:stylesheet>
