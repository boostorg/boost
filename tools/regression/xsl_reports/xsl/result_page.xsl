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

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

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

  <!-- the author-specified expected test results -->
  <xsl:variable name="expected_results" select="document( $expected_results_file )" />

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
  <xsl:variable name="toolsets" select="//test-log[generate-id(.) = generate-id( key('toolset_key',@toolset)[1] )]/@toolset"/>
  <xsl:variable name="libraries" select="//test-log[ generate-id(.) = generate-id( key('library_key',@library)[1] ) and @library != '' ]/@library"/>

  <xsl:template name="toolsets_row">
    <tr>
      <td class="head" colspan="2">test / toolset</td>
      <xsl:for-each select="$toolsets">
        <xsl:sort select="." order="ascending" />
        <td class="toolset-name"><xsl:value-of select="."/></td>
      </xsl:for-each>
      <td class="head">toolset / test</td>
    </tr>
  </xsl:template>

  <xsl:template name="test_type_col">
    <td class="test-type">
      <a href="#legend" class="legend-link">
        <xsl:variable name="test_type" select="./@test-type"/>
        <xsl:choose>
          <xsl:when test="$test_type='run'">
            <xsl:text>r</xsl:text>
          </xsl:when>
          <xsl:when test="$test_type='run_fail'">
            <xsl:text>rf</xsl:text>
          </xsl:when>
          <xsl:when test="$test_type='compile'">
            <xsl:text>c</xsl:text>
          </xsl:when>
          <xsl:when test="$test_type='compile_fail'">
            <xsl:text>cf</xsl:text>
          </xsl:when>
        </xsl:choose>
      </a>
    </td>
  </xsl:template>


  <xsl:template match="/">
    <html>
      <head>
        <link rel="stylesheet" type="text/css" href="master.css" title="master" />
        <title>Boost regression - <xsl:value-of select="$mode"/> detailed report: <xsl:value-of select="$source"/></title>
      </head>
      <body>

        <!-- header -->

        <xsl:variable name="legend">
          <xsl:choose>
            <xsl:when test="$mode='user'">
              <tr>
                <td class="header-item">Legend</td>
                <td class="header-item-content">

                  <table border="0" class="legend-table">
                    <tr>
                      <td><table><tr><td class="result-user-success">pass</td></tr></table></td>
                      <td class="legend-item">the test passes</td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-user-fail-expected">fail</td></tr></table></td>
                      <td class="legend-item">
                        a known test failure; click on the link to see the log
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-user-fail-unexpected">unexp.</td></tr></table></td>
                      <td class="legend-item">
                        the test is known to pass, but is currently failing; 
                        click on the link to see the log
                      </td>
                    </tr>
                  </table>

                </td>
              </tr>
            </xsl:when>
            <xsl:when test="$mode='developer'">
              <tr>
                <td class="header-item">Legend</td>
                <td class="header-item-content">
                  <table border="0" class="legend-table">
                    <tr>
                      <td><table><tr><td class="result-success-expected">pass</td></tr></table></td>
                      <td>
                        success
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-success-unexpected">pass</td></tr></table></td>
                      <td>
                        unexpected success
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-fail-expected">fail</td></tr></table></td>
                      <td>
                        expected failure
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-fail-unexpected-new">fail</td></tr></table></td>
                      <td class="legend-item">
                        a failure on a newly added test or compiler
                      </td>
                    </tr>
                    <tr>
                      <td><table><tr><td class="result-fail-unexpected">fail</td></tr></table></td>
                      <td>unexpected failure</td>
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
                  <xsl:text> detailed report: </xsl:text>
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
            <tr>
              <td class="header-item">Purpose</td>
              <td class="header-item-content">
                Provides detailed explanation of the results reported on the
                <a href="{$mode}_summary_page.html"><xsl:value-of select="$mode"/> summary</a> page.
              </td>
            </tr>
            <xsl:copy-of select="$legend"/>
          </table>
          <div class="legend">
            To specify the expected failures for specific test cases create an .xml file 
            "boost/status/&lt;your library&gt;/expected_results.xml" along these lines:
<pre>
&lt;expected-failures&gt;
&lt;!-- Example:
&lt;test-result library="mpl" test-name="is_sequence" toolset="bcc-5.5.1" /&gt;
&lt;test-result library="mpl" test-name="is_sequence" toolset="cwpro-8.3" /&gt;
--&gt;
            
&lt;/expected-failures&gt;
</pre>
         </div>
      </div>

      <!-- TOC -->
      <div class="toc">
        <a name="toc"></a>
        <ul>
          <xsl:for-each select="$libraries">
            <xsl:sort select="." order="ascending" />
            <li class="toc-entry">
              <a href="#{.}" class="toc-entry">
                <xsl:value-of select="."/>
              </a>
            </li>
          </xsl:for-each>
        </ul>
      </div>
      
      <!-- for each library -->
      <xsl:for-each select="$libraries">
        <xsl:sort select="." order="ascending" />
        <xsl:variable name="library" select="." />
        <div class="library-name">
            <a name="{$library}" href="../libs/{$library}" class="library-link">
              <xsl:value-of select="$library" />
            </a>
          </div>

          <table border="1" cellspacing="0" cellpadding="0" class="detail-table">

            <thead><xsl:call-template name="toolsets_row"/></thead>
            <tfoot><xsl:call-template name="toolsets_row"/></tfoot>
      
            <tbody>
              <xsl:variable name="lib_tests" select="//test-log[@library = $library]" />
              <xsl:variable name="lib_unique_test_names" 
                select="$lib_tests[ generate-id(.) = generate-id( key('test_name_key', concat( @library, '&gt;@&lt;', @test-name ) ) ) ]" />
            
              <xsl:for-each select="$lib_unique_test_names">
                <xsl:variable name="test_name" select="./@test-name"/>
                <xsl:variable name="test_program" select="./@test-program"/>
                <xsl:variable name="test_header">
                  <td class="test-name">
                    <a href="../{$test_program}" class="test-link">
                      <xsl:value-of select="$test_name"/>
                    </a>
                  </td>
                </xsl:variable>
                <tr>
                  <xsl:copy-of select="$test_header"/>
                  <xsl:call-template name="test_type_col"/>

                  <xsl:for-each select="$lib_tests[ @test-name = $test_name ]">
                    <xsl:sort select="@toolset" order="ascending" />
                    <xsl:variable name="toolset" select="@toolset" />

                    <xsl:choose>
                      <xsl:when test="$mode='user'">
                        <xsl:call-template name="insert_cell_user">
                          <xsl:with-param name="test_log" select="."/>
                          <xsl:with-param name="log_link" select="concat( $links_file, '#', $test_name, '-', $toolset )"/>
                        </xsl:call-template>
                      </xsl:when>
                      <xsl:when test="$mode='developer'">
                        <xsl:call-template name="insert_cell_developer">
                          <xsl:with-param name="test_log" select="."/>
                          <xsl:with-param name="log_link" select="concat( $links_file, '#', $test_name, '-', $toolset )"/>
                        </xsl:call-template>
                      </xsl:when>
                    </xsl:choose>

                  </xsl:for-each>
                  <xsl:copy-of select="$test_header"/>
                </tr>
              </xsl:for-each>
            </tbody>
          </table>
          <div class="footer">
            <a href="#toc" class="back-link">toc</a>
            <xsl:text>&#160;|&#160;</xsl:text>
            <a href="{$mode}_summary_page.html" class="back-link">summary</a>
          </div>
        </xsl:for-each>
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
    <xsl:param name="test_log"/>
    <xsl:param name="log_link"/>
    
    <xsl:variable name="is_new">
       <xsl:if test="$test_log/@is-new = 'yes' and $test_log/@status = 'unexpected' and $test_log/@result != 'success'">
          <xsl:value-of select="'-new'"/>
       </xsl:if>
    </xsl:variable>
    <td class="result-{$test_log/@result}-{$test_log/@status}{$is_new}">
      <xsl:choose>
        <xsl:when test="$test_log/@result != 'success' and $test_log/@status = 'expected'">
          <a href="{$log_link}" class="log-link">
            fail
          </a>
        </xsl:when>
        <xsl:when test="$test_log/@result != 'success' and $test_log/@status = 'unexpected'">
          <a href="{$log_link}" class="log-link">
            fail
          </a>
        </xsl:when>
        <xsl:when test="$test_log/@result = 'success' and $test_log/@status = 'unexpected'">
            pass
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>pass</xsl:text>
        </xsl:otherwise>
      </xsl:choose>  
    </td>
  </xsl:template>

  <!-- report user status -->
  <xsl:template name="insert_cell_user">
    <xsl:param name="test_log"/>
    <xsl:param name="log_link"/>
    
    <xsl:variable name="class">
      <xsl:choose>
        <xsl:when test="$test_log[@result='fail' and @status='unexpected']">
          <xsl:text>result-user-fail-unexpected</xsl:text>
        </xsl:when>
        <xsl:when test="$test_log[ @result='fail' and @status='expected' ]">
        <xsl:text>result-user-fail-expected</xsl:text>
      </xsl:when>
      <xsl:when test="$test_log[ @result='success']">
        <xsl:text>result-user-success</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          Unknown status
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

    <td class="{$class}">
      <xsl:choose>
        <xsl:when test="$test_log/@result != 'success' and $test_log/@status = 'expected'">
          <a href="{$log_link}" class="log-link">
            fail
          </a>
        </xsl:when>
        <xsl:when test="$test_log/@result != 'success'">
          <a href="{$log_link}" class="log-link">
            unexp.
          </a>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>pass</xsl:text>
        </xsl:otherwise>
      </xsl:choose>  
    </td>
  </xsl:template>


</xsl:stylesheet>
