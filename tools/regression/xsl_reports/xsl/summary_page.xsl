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
  version="1.0">

  <xsl:output method="html" 
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" 
    encoding="utf-8" 
    indent="yes"
    />

  <xsl:param name="mode"/>
  <xsl:param name="source"/>
  <xsl:param name="run_date"/>
  <xsl:param name="comment_file"/>

  <xsl:variable name="alternate_mode">
    <xsl:choose>
      <xsl:when test="$mode='user'">developer</xsl:when>
      <xsl:otherwise>user</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <!-- necessary indexes -->
  <xsl:key 
    name="test_name_key" 
    match="test-log" 
    use="concat( @library, '&gt;@&lt;', @test-name )"/>
  <xsl:key 
    name="library_key" 
    match="test-log" 
    use="@library"/>
  <xsl:key name="toolset_key" match="test-log" use="@toolset"/>

  <!-- -->
  <xsl:variable name="toolsets" select="//test-log[ generate-id(.) = generate-id( key('toolset_key',@toolset)[1] ) ]/@toolset"/>
  <xsl:variable name="libraries" select="//test-log[ generate-id(.) = generate-id( key('library_key',@library)[1] ) and @library != '' ]/@library"/>

  <xsl:template name="toolsets_row">
    <tr>
      <td class="head">library / toolset</td>
      <xsl:for-each select="$toolsets">
        <xsl:sort select="." order="ascending" />
        <td class="toolset-name"><xsl:value-of select="."/></td>
      </xsl:for-each>
      <td class="head">toolset / library</td>
    </tr>
  </xsl:template>

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
                      <td><table><tr><td class="summary-user-success">&#160;</td></tr></table></td>
                      <td class="legend-item">all library tests are passing</td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="summary-user-fail-expected">details</td></tr></table></td>
                      <td class="legend-item">
                        there are some known failures in the tests, see the detailed report
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="summary-user-fail-unexpected">unexp.</td></tr></table></td>
                      <td class="legend-item">
                        some tests that the library author expects to pass are currently failing,
                        see the detailed report
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
                      <td><table><tr><td class="summary-expected">OK</td></tr></table></td>
                      <td class="legend-item">all expected tests pass</td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="summary-success-unexpected">OK</td></tr></table></td>
                      <td class="legend-item">
                        all expected tests pass, and some other tests that were expected to fail 
                        unexpectedly pass as well
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="summary-fail-unexpected-new">fail</td></tr></table></td>
                      <td class="legend-item">
                        there are some failures on the newly added tests/compiler(s)
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="summary-fail-unexpected">broken</td></tr></table></td>
                      <td class="legend-item">
                        tests that the library author expects to pass are currently failing
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
              <td><img border="0" src="../c++boost.gif" width="277" height="86" alt="Boost logo"></img></td>
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

        <table border="1" cellspacing="0" cellpadding="0" class="summary-table">

          <thead><xsl:call-template name="toolsets_row"/></thead>
          <tfoot><xsl:call-template name="toolsets_row"/></tfoot>
      
          <tbody>
            <!-- for each library -->
            <xsl:for-each select="$libraries">
              <xsl:sort select="." order="ascending" />
              <xsl:variable name="library" select="."/>
              <xsl:variable name="current_row" select="//test-log[ @library=$library]"/>

              <xsl:variable name="library_header">
                <td class="library-name">
                  <a href="{$mode}_result_page.html#{.}" class="library-link">
                    <xsl:value-of select="."/>
                  </a>
                </td>
              </xsl:variable>

              <tr class="summary-row">
                <xsl:copy-of select="$library_header"/>

                <xsl:for-each select="$toolsets">
                  <xsl:sort select="." order="ascending" />
                  <xsl:variable name="toolset" select="."/>

                  <xsl:variable name="current_cell" select="$current_row[ @toolset=$toolset ]"/>
                  <xsl:choose>
                    <xsl:when test="$mode='user'">
                      <xsl:call-template name="insert_cell_user">
                        <xsl:with-param name="current_cell" select="$current_cell"/>
                        <xsl:with-param name="library" select="$library"/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="$mode='developer'">
                      <xsl:call-template name="insert_cell_developer">
                        <xsl:with-param name="current_cell" select="$current_cell"/>
                        <xsl:with-param name="library" select="$library"/>
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
  <xsl:variable name="class">
    <xsl:choose>
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
    </xsl:choose>
  </xsl:variable>
  
  <td class="{$class}">
    <xsl:choose>
      <xsl:when test="$class='summary-fail-unexpected' or $class='summary-fail-unexpected-new' ">
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
  <xsl:variable name="class">
    <xsl:choose>
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

</xsl:stylesheet>
