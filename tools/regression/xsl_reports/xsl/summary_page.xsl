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
  version="1.0">

  <xsl:import href="common.xsl"/>

  <xsl:output method="html" 
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" 
    encoding="utf-8" 
    indent="yes"
    />


  <xsl:param name="mode"/>
  <xsl:param name="source"/>
  <xsl:param name="run_date"/>
  <xsl:param name="comment_file"/>
  <xsl:param name="explicit_markup_file"/>

  <xsl:variable name="explicit_markup" select="document( $explicit_markup_file )"/>

  <xsl:variable name="alternate_mode">
    <xsl:choose>
      <xsl:when test="$mode='user'">developer</xsl:when>
      <xsl:otherwise>user</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <!-- necessary indexes -->
  <xsl:key 
    name="library_test_name_key" 
    match="test-log" 
    use="concat( @library, '&gt;@&lt;', @test-name )"/>
  <xsl:key 
    name="library_key" 
    match="test-log" 
    use="@library"/>
  <xsl:key name="toolset_key" match="test-log" use="@toolset"/>
  <xsl:key name="test_name_key"  match="test-log" use="@test-name "/>

  <!-- toolsets -->

  <xsl:variable name="toolsets" select="//test-log[ generate-id(.) = generate-id( key('toolset_key',@toolset)[1] ) and @toolset != '' ]/@toolset"/>

  <xsl:variable name="required_toolsets" select="$explicit_markup//mark-toolset[ @status='required' ]"/>

  <xsl:variable name="sorted_toolset_fragment">
    <xsl:call-template name="get_toolsets">
      <xsl:with-param name="toolsets" select="$toolsets"/>
      <xsl:with-param name="required_toolsets" select="$required_toolsets"/>
    </xsl:call-template>
  </xsl:variable>

  <xsl:variable name="sorted_toolsets" select="exsl:node-set( $sorted_toolset_fragment )"/>
    
  <!-- libraries -->

  <xsl:variable name="libraries" select="//test-log[ generate-id(.) = generate-id( key('library_key',@library)[1] ) and @library != '' ]/@library"/>

  <xsl:variable name="sorted_libraries_output">
      <xsl:for-each select="$libraries">
        <xsl:sort select="." order="ascending" />
        <library><xsl:copy-of select="."/></library>
      </xsl:for-each>
  </xsl:variable>

  <xsl:variable name="sorted_libraries" select="exsl:node-set( $sorted_libraries_output )/library/@library"/>

  
  <xsl:template match="/">
    <html>
      <head>
        <link rel="stylesheet" type="text/css" href="master.css" title="master" />
        <title>Boost regression - <xsl:value-of select="$mode"/> summary: <xsl:value-of select="$source"/></title>
      </head>
      <body>

        <!-- header -->

        <xsl:variable name="legend">
          <xsl:choose>
            <xsl:when test="$mode='user'">
              <tr>
                <td class="header-item">Purpose</td>
                <td class="header-item-content">
                  The purpose of this page and the <a href="{$mode}_result_page.html">detailed user report</a>
                  is to help a user to find out whether a particular library works on the particular compiler(s).
                  For CVS "health report", see <a href="{$alternate_mode}_summary_page.html">developer summary</a>.
                </td>
              </tr>
              <tr>
                <td class="header-item">Legend</td>
                <td class="header-item-content">
                  <table border="0" class="legend-table">
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-user-success">&#160;</td></tr></table></td>
                      <td class="legend-item">all library tests are passing</td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-user-fail-expected">details</td></tr></table></td>
                      <td class="legend-item">
                        there are some known failures in the tests, see the detailed report
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-user-fail-unexpected">unexp.</td></tr></table></td>
                      <td class="legend-item">
                        some tests that the library author expects to pass are currently failing,
                        see the detailed report
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-unusable">unusable</td></tr></table></td>
                      <td class="legend-item">
                        the library author marked it as unusable on particular platform/toolset
                      </td>
                    </tr>
                  </table>
                </td>
              </tr>
            </xsl:when>
            <xsl:when test="$mode='developer'">
              <tr>
                <td class="header-item">Purpose</td>
                <td class="header-item-content">
                  The purpose of this page is to provide boost developers with visual indication
                  of the CVS "health". 
                  <a href="{$mode}_result_page.html">Detailed report</a> is available.
                  For user-level report, see <a href="{$alternate_mode}_summary_page.html">user summary</a>.
                </td>
              </tr>
              <tr>
                <td class="header-item">Legend</td>
                <td class="header-item-content">
                  <table border="0" class="legend-table">
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-expected">OK</td></tr></table></td>
                      <td class="legend-item">
                        all expected tests pass
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-success-unexpected">OK</td></tr></table></td>
                      <td class="legend-item">
                        all expected tests pass, and some other tests that were expected to fail 
                        unexpectedly pass as well
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-fail-unexpected-new">fail</td></tr></table></td>
                      <td class="legend-item">
                        there are some failures on the newly added tests/compiler(s)
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-fail-unexpected">broken</td></tr></table></td>
                      <td class="legend-item">
                        tests that the library author expects to pass are currently failing
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%"><tr class="summary-row-single"><td class="summary-unusable">unusable</td></tr></table></td>
                      <td class="legend-item">
                        the library author marked it as unusable on particular platform/toolset
                      </td>
                    </tr>
                  </table>
                </td>
              </tr>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>

        <div>
          <table border="0">
            <tr>
              <td><img border="0" src="../../c++boost.gif" width="277" height="86" alt="Boost logo"></img></td>
              <td>
                <h1 class="page-title">
                  <xsl:value-of select="$mode"/>
                  <xsl:text> summary: </xsl:text>
                  <xsl:value-of select="$source"/>
                </h1>
                <b>Report Time: </b> <xsl:value-of select="$run_date"/>
                <xsl:if test="$comment_file != ''">
                  <xsl:copy-of select="document( $comment_file )"/>
                </xsl:if>
              </td>
            </tr>
          </table>
          <table border="0" class="header-table">
            <xsl:copy-of select="$legend"/>
          </table>
        </div>

        <!-- summary table -->

        <table border="0" cellspacing="0" cellpadding="0" class="summary-table">

          <thead>
            <xsl:call-template name="insert_toolsets_row">
              <xsl:with-param name="toolsets" select="$sorted_toolsets"/>
            </xsl:call-template>
          </thead>

          <tfoot>
            <xsl:call-template name="insert_toolsets_row">
              <xsl:with-param name="toolsets" select="$sorted_toolsets"/>
            </xsl:call-template>
          </tfoot>
      
          <tbody>
            <xsl:variable name="test_logs" select="//test-log"/>

            <!-- for each library -->
            <xsl:for-each select="$sorted_libraries">
              <xsl:variable name="library" select="."/>
              <xsl:variable name="current_row" select="$test_logs[ @library=$library ]"/>

              <xsl:variable name="expected_test_count" select="count( $current_row[ generate-id(.) = generate-id( key('test_name_key',@test-name)[1] ) ] )"/>
              <xsl:variable name="library_header">
                <td class="library-name">
                  <a href="{$mode}_result_page.html#{.}" class="library-link">
                    <xsl:value-of select="$library"/>
                  </a>
                </td>
              </xsl:variable>

              <xsl:variable name="line_mod">
                <xsl:choose>
                  <xsl:when test="1 = last()">
                    <xsl:text>-single</xsl:text>
                  </xsl:when>
                  <xsl:when test="generate-id( . ) = generate-id( $sorted_libraries[1] )">
                    <xsl:text>-first</xsl:text>
                  </xsl:when>
                  <xsl:when test="generate-id( . ) = generate-id( $sorted_libraries[ last() ] )">
                    <xsl:text>-last</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text></xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:variable>


              <tr class="summary-row{$line_mod}">
                <xsl:copy-of select="$library_header"/>

                <xsl:for-each select="$sorted_toolsets/toolset">
                  <xsl:variable name="toolset" select="@toolset"/>
                  <xsl:variable name="current_cell" select="$current_row[ @toolset=$toolset ]"/>
                  <xsl:choose>
                    <xsl:when test="$mode='user'">
                      <xsl:call-template name="insert_cell_user">
                        <xsl:with-param name="current_cell" select="$current_cell"/>
                        <xsl:with-param name="library" select="$library"/>
                        <xsl:with-param name="toolset" select="$toolset"/>
                        <xsl:with-param name="expected_test_count" select="$expected_test_count"/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="$mode='developer'">
                      <xsl:call-template name="insert_cell_developer">
                        <xsl:with-param name="current_cell" select="$current_cell"/>
                        <xsl:with-param name="library" select="$library"/>
                        <xsl:with-param name="toolset" select="$toolset"/>
                        <xsl:with-param name="expected_test_count" select="$expected_test_count"/>
                      </xsl:call-template>
                    </xsl:when>
                  </xsl:choose>
                </xsl:for-each>
                
                <xsl:copy-of select="$library_header"/>
              </tr>          
            </xsl:for-each>
          </tbody>
        </table>
        <div>
          <a href="http://validator.w3.org/check/referer">
            <img border="0" src="http://www.w3.org/Icons/valid-html401" alt="Valid HTML 4.01!" height="31" width="88"/>
          </a>
        </div>
    </body>
 </html>
</xsl:template>

<!-- report developer status -->
<xsl:template name="insert_cell_developer">
  <xsl:param name="current_cell"/>
  <xsl:param name="library"/>
  <xsl:param name="toolset"/>
  <xsl:param name="expected_test_count"/>
  <xsl:variable name="class">
    <xsl:choose> 
      <xsl:when test="meta:is_unusable( $explicit_markup, $library, $toolset )">
        <xsl:text>summary-unusable</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell ) &lt; $expected_test_count">
        <xsl:text>summary-missing</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[@result='fail' and  @status='unexpected' and @is-new='no'] )">
        <xsl:text>summary-fail-unexpected</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[@result='fail' and  @status='unexpected' and @is-new='yes'] )">
        <xsl:text>summary-fail-unexpected-new</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[@result='success' and  @status='unexpected'] )">
        <xsl:text>summary-success-unexpected</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[@status='expected'] )">
        <xsl:text>summary-expected</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="count( $current_cell )"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  
  <td class="{$class}">
    <xsl:choose>
      <xsl:when test="$class='summary-unusable'">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>unusable</xsl:text>
        </a>          
      </xsl:when>
      <xsl:when test="$class='summary-missing'">
        <xsl:text>missing</xsl:text>
      </xsl:when>
      <xsl:when test="$class='summary-fail-unexpected'">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>broken</xsl:text>
        </a>
      </xsl:when>
      <xsl:when test="$class='summary-fail-unexpected-new' ">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>fail</xsl:text>
        </a>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>OK</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </td>
  
</xsl:template>


<!-- report user status -->
<xsl:template name="insert_cell_user">
  <xsl:param name="current_cell"/>
  <xsl:param name="library"/>
  <xsl:param name="toolset"/>
  <xsl:param name="expected_test_count"/>
  <xsl:variable name="class">
    <xsl:choose>
      <xsl:when test="meta:is_unusable( $explicit_markup, $library, $toolset )">
        <xsl:text>summary-unusable</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell ) &lt; $expected_test_count">
        <xsl:text>summary-missing</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[@result='fail' and @status='unexpected' ] )">
        <xsl:text>summary-user-fail-unexpected</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[ @result='fail'] )">
        <xsl:text>summary-user-fail-expected</xsl:text>
      </xsl:when>
      <xsl:when test="count( $current_cell[ @result='success'] )">
        <xsl:text>summary-user-success</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          Unknown status
          <xsl:copy-of select="$current_cell">
          </xsl:copy-of>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  
  <td class="{$class}">
    <xsl:choose>
      <xsl:when test="$class='summary-unusable'">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>unusable</xsl:text>
        </a>          
      </xsl:when>

      <xsl:when test="$class='summary-missing'">
        <xsl:text>missing</xsl:text>
      </xsl:when>

      <xsl:when test="$class='summary-user-fail-unexpected'">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>unexp.</xsl:text>
        </a>
      </xsl:when>

      <xsl:when test="$class='summary-user-fail-expected'">
        <a href="{$mode}_result_page.html#{$library}" class="log-link">
          <xsl:text>details</xsl:text>
        </a>
      </xsl:when>

      <xsl:otherwise>
        <xsl:text>&#160;</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </td>
  
</xsl:template>

<xsl:template name="insert_toolsets_row">
  <xsl:param name="toolsets"/>
  <tr>
    <td class="head">library / toolset</td>
    
    <xsl:for-each select="$toolsets/toolset">
      <xsl:variable name="class">
        <xsl:choose>
          <xsl:when test="@required='yes'">
            <xsl:text>required-toolset-name</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>toolset-name</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      
      <td class="{$class}"><xsl:value-of select="@toolset"/></td>
    </xsl:for-each>

    <td class="head">toolset / library</td>
  </tr>
</xsl:template>

</xsl:stylesheet>
