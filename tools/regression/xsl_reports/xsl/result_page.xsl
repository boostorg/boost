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

<xsl:stylesheet 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:exsl="http://exslt.org/common"
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
    use="concat( @library, '&gt;@&lt;', @test-name )"/>
  <xsl:key 
    name="library_key" 
    match="test-log" 
    use="@library"/>
  <xsl:key name="toolset_key" match="test-log" use="@toolset"/>

  <!-- toolsets -->

  <xsl:variable name="not_ordered_toolsets" select="//test-log[ generate-id(.) = generate-id( key('toolset_key',@toolset)[1] ) and @toolset != '' ]/@toolset"/>

  <xsl:variable name="required_toolsets" select="$explicit_markup//mark-toolset[ @status='required' ]"/>

  <xsl:variable name="ordered_toolsets_fragment">
    <xsl:call-template name="get_toolsets">
      <xsl:with-param name="toolsets" select="$not_ordered_toolsets"/>
      <xsl:with-param name="required_toolsets" select="$required_toolsets"/>
    </xsl:call-template>
  </xsl:variable>

  <xsl:variable name="ordered_toolsets" select="exsl:node-set( $ordered_toolsets_fragment )"/>

  <!-- libraries -->
  <xsl:variable name="libraries" select="//test-log[ @library != '' and generate-id(.) = generate-id( key('library_key',@library)[1] )  ]/@library"/>

  <xsl:template name="insert_toolsets_row">
    <xsl:param name="library"/>
    <xsl:param name="library_marks"/>
    <xsl:param name="toolsets"/>
    <tr valign="middle">
      <td class="head" colspan="2">test / toolset</td>

      <xsl:variable name="library_notes" select="$library_marks/note"/>
      <xsl:for-each select="$toolsets/toolset">
        <xsl:variable name="toolset" select="@toolset"/>

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
          
        <td class="{$class}"><xsl:value-of select="$toolset"/>
          <xsl:if test="$library_notes">
            <span class="super">
              <xsl:for-each select="$library_notes">
                <xsl:if test="../@toolset=$toolset or ( ../toolset/@name=$toolset or ../toolset/@name = '*' )">
                  <xsl:variable name="note_index" select="position()"/>
                  <a href="#{$library}-note-{$note_index}" title="Note {$note_index}"><xsl:value-of select="$note_index"/></a>
                </xsl:if>
              </xsl:for-each>
            </span>
          </xsl:if>
        </td>
      </xsl:for-each>
      <td class="head">toolset / test</td>
    </tr>
  </xsl:template>

  <xsl:template name="test_type_col">
    <td class="test-type">
      <a href="#legend" class="legend-link">
        <xsl:variable name="test_type" select="./@test-type"/>
        <xsl:choose>
          <xsl:when test="$test_type='run_pyd'">
            <xsl:text>r</xsl:text>
          </xsl:when>
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

                  <table border="0" cellspacing="0" cellpadding="0" class="legend-table" summary="legend table">
                    <tr>
                      <td><table width="100%" summary="success legend"><tr class="detail-row-single"><td class="detail-user-success">pass</td></tr></table></td>
                      <td class="legend-item">
                        the test passes
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="fail legend"><tr><td class="detail-user-fail-expected">fail</td></tr></table></td>
                      <td class="legend-item">
                        a known test failure; click on the link to see the log
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unexpected fail legend"><tr><td class="detail-user-fail-unexpected">unexp.</td></tr></table></td>
                      <td class="legend-item">
                        the test is known to pass, but is currently failing; 
                        click on the link to see the log
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unusable legend"><tr><td class="detail-unusable">...</td></tr></table></td>
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
                <td class="header-item">Legend</td>
                <td class="header-item-content">
                  <table border="0" class="legend-table" summary="legend">
                    <tr>
                      <td><table width="100%" summary="success legend"><tr class="detail-row-single"><td class="detail-success-expected">pass</td></tr></table></td>
                      <td class="legend-item">
                        success
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unexpected pass legend"><tr class="detail-row-single"><td class="detail-success-unexpected">pass</td></tr></table></td>
                      <td class="legend-item">
                        unexpected success
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="expected fail legend"><tr class="detail-row-single"><td class="detail-fail-expected">fail</td></tr></table></td>
                      <td class="legend-item">
                        expected failure
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unexpected new fail legend"><tr class="detail-row-single"><td class="detail-fail-unexpected-new">fail</td></tr></table></td>
                      <td class="legend-item">
                        failure on a newly added test/compiler
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unexpected fail legend"><tr class="detail-row-single"><td class="detail-fail-unexpected">fail</td></tr></table></td>
                      <td class="legend-item">
                        unexpected failure
                      </td>
                    </tr>
                    <tr>
                      <td><table width="100%" summary="unusable legend"><tr class="detail-row-single"><td class="detail-unusable">...</td></tr></table></td>
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
          <table border="0" summary="report header">
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
          <table border="0" class="header-table" summary="report description" >
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
            To specify the explicit markup edit status/explicit-failure-markup.xml
         </div>
      </div>

      <!-- TOC -->
      <div class="detail-toc">
        <a name="toc"></a>
        <ul>
          <xsl:for-each select="$libraries">
            <xsl:sort select="." order="ascending" />
            <li class="detail-toc-entry">
              <a href="#{.}" class="detail-toc-entry">
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

          <xsl:variable name="library_marks" select="$explicit_markup//mark-library[ @name = $library ]"/>

          <table border="0" cellspacing="0" cellpadding="0" class="detail-table" summary="library results">

            <thead>
              <xsl:call-template name="insert_toolsets_row">
                <xsl:with-param name="library_marks" select="$library_marks"/>
                <xsl:with-param name="library" select="$library"/>
                <xsl:with-param name="toolsets" select="$ordered_toolsets"/>
              </xsl:call-template>
            </thead>
            <tfoot>
              <xsl:call-template name="insert_toolsets_row">
                <xsl:with-param name="library_marks" select="$library_marks"/>
                <xsl:with-param name="library" select="$library"/>
                <xsl:with-param name="toolsets" select="$ordered_toolsets"/>
              </xsl:call-template>
            </tfoot>

            <tbody>
              <xsl:variable name="lib_tests" select="//test-log[@library = $library]" />
              <xsl:variable name="lib_unique_test_names" 
                select="$lib_tests[ generate-id(.) = generate-id( key('test_name_key', concat( @library, '&gt;@&lt;', @test-name ) ) ) ]" />
              <xsl:variable name="lib_corner_case_tests_markup" select="$explicit_markup//mark-test[ @as='corner case' and @library = $library ]"/>
              
              <xsl:variable name="lib_general_tests" select="$lib_unique_test_names[ not( @test-name = $lib_corner_case_tests_markup/@test )  ]"/>

              <xsl:variable name="lib_corner_case_tests" select="$lib_unique_test_names[ @test-name = $lib_corner_case_tests_markup/@test ] " />
            
              <xsl:for-each select="$lib_general_tests">
                <xsl:variable name="test_name" select="./@test-name"/>
                <xsl:variable name="line_mod">
                  <xsl:choose>
                    <xsl:when test="1 = last()">
                      <xsl:text>-single</xsl:text>
                    </xsl:when>
                    <xsl:when test="generate-id( . ) = generate-id( $lib_general_tests[1] )">
                      <xsl:text>-first</xsl:text>
                    </xsl:when>
                    <xsl:when test="generate-id( . ) = generate-id( $lib_general_tests[last()] )">
                      <xsl:text>-last</xsl:text>
                    </xsl:when>
                  <xsl:otherwise>
                    <xsl:text></xsl:text>
                  </xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>

                <xsl:call-template name="insert-test-line">
                  <xsl:with-param name="test_results" select="$lib_tests[ @test-name = $test_name ]"/>
                  <xsl:with-param name="toolsets" select="$ordered_toolsets"/>
                  <xsl:with-param name="test_name" select="$test_name"/>
                  <xsl:with-param name="line_mod" select="$line_mod"/>
                </xsl:call-template>
              </xsl:for-each>

              <xsl:if test="count( $lib_corner_case_tests ) > 0">
                <tr>
                  <!--<td colspan="2">&#160;</td>                  -->
                  <td class="detail-corner-case-header" colspan="{count($ordered_toolsets) + 3 }" align="center">Corner-case tests</td>
                  <!--<td>&#160;</td>-->
                </tr>
                
                <xsl:for-each select="$lib_corner_case_tests">
                  <xsl:variable name="test_name" select="@test-name"/>

                  <xsl:variable name="line_mod">
                    <xsl:choose>
                      <xsl:when test="1 = last()">
                        <xsl:text>-single</xsl:text>
                      </xsl:when>
                      <xsl:when test="generate-id( . ) = generate-id( $lib_general_tests[1] )">
                        <xsl:text>-first</xsl:text>
                      </xsl:when>
                      <xsl:when test="generate-id( . ) = generate-id( $lib_general_tests[last()] )">
                        <xsl:text>-last</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text></xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:variable>

                  
                  <xsl:call-template name="insert-test-line">
                    <xsl:with-param name="test_results" select="$lib_tests[ @test-name = $test_name ]"/>
                    <xsl:with-param name="toolsets" select="$ordered_toolsets"/>
                    <xsl:with-param name="test_name" select="$test_name"/>
                    <xsl:with-param name="line_mod" select="$line_mod"/>
                  </xsl:call-template>
                </xsl:for-each>
                
              </xsl:if>

            </tbody>
          </table>
          <xsl:if test="count( $library_marks ) > 0 ">
            <table border="0" cellpadding="0" cellspacing="0" class="detail-library-notes" summary="library notes">
              <xsl:for-each select="$library_marks/note">
                <tr class="detail-library-note">
                  <td valign="top" width="3em">
                    <a name="{$library}-note-{position()}">
                      <span class="super"><xsl:value-of select="position()"/></span>
                    </a>
                  </td>
                  <td>
                    <span class="detail-library-note-header">
                      <xsl:choose>
                        <xsl:when test="@author and @date">
                          [&#160;<xsl:value-of select="@author"/>&#160;<xsl:value-of select="@date"/>&#160;]
                        </xsl:when>
                        <xsl:when test="@author">
                          [&#160;<xsl:value-of select="@author"/>&#160;]                        
                        </xsl:when>
                        <xsl:when test="@date">
                          [&#160;<xsl:value-of select="@date"/>&#160;]                        
                        </xsl:when>
                      </xsl:choose>
                    </span>
                    <xsl:copy-of select="node()"/>
                  </td>
                </tr>
              </xsl:for-each>
            </table>
          </xsl:if>
          <div class="detail-footer">
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

    <xsl:variable name="class">
      <xsl:choose>
        <xsl:when test="not( $test_log )">
          <xsl:text>detail-missing</xsl:text>
        </xsl:when>
        <xsl:when test="$explicit_markup//mark-library[ @name=$test_log/@library and ( @toolset = $test_log/@toolset or toolset/@name = $test_log/@toolset )  ]">
          <xsl:text>detail-unusable</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat( 'detail-', $test_log/@result, '-', $test_log/@status, $is_new )"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <td class="{$class}">
      <xsl:choose>
        <xsl:when test="not( $test_log )">
          <xsl:text>missing</xsl:text>
        </xsl:when> 
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
      <xsl:if test="count( $test_log ) > 1" > 
        <div class="detail-conf-problem">conf.&#160;problem</div>
      </xsl:if>
    </td>
  </xsl:template>

  <!-- report user status -->
  <xsl:template name="insert_cell_user">
    <xsl:param name="test_log"/>
    <xsl:param name="log_link"/>
    
    <xsl:variable name="class">
      <xsl:choose>
        <xsl:when test="not( $test_log )">
          <xsl:text>detail-missing</xsl:text>
        </xsl:when>
        <xsl:when test="$explicit_markup//mark-library[ @name=$test_log/@library and ( @toolset = $test_log/@toolset or toolset/@name = $test_log/@toolset )  ]">
          <xsl:text>detail-unusable</xsl:text>
        </xsl:when>
        <xsl:when test="$test_log[@result='fail' and @status='unexpected']">
          <xsl:text>detail-user-fail-unexpected</xsl:text>
        </xsl:when>
        <xsl:when test="$test_log[ @result='fail' and @status='expected' ]">
        <xsl:text>detail-user-fail-expected</xsl:text>
      </xsl:when>
      <xsl:when test="$test_log[ @result='success']">
        <xsl:text>detail-user-success</xsl:text>
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
        <xsl:when test="not( $test_log )">
          missing
        </xsl:when>
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

      <xsl:if test="count( $test_log ) > 1" > 
        <div class="conf-problem">conf.&#160;problem</div>
      </xsl:if>
    </td>
  </xsl:template>

  <xsl:template name="insert-test-line">
    <xsl:param name="test_name"/>
    <xsl:param name="test_results"/>
    <xsl:param name="toolsets"/>
    <xsl:param name="line_mod"/>

    <xsl:variable name="test_program">
      <xsl:value-of select="$test_results[1]/@test-program"/>
    </xsl:variable>

    <xsl:variable name="test_header">
      <td class="test-name">
        <a href="{$test_program}" class="test-link">
          <xsl:value-of select="$test_name"/>
        </a>
      </td>
    </xsl:variable>

    <tr class="detail-row{$line_mod}">
      <xsl:copy-of select="$test_header"/>
      <xsl:call-template name="test_type_col"/>
      
      <xsl:for-each select="$toolsets/toolset">
        <xsl:variable name="toolset" select="@toolset" />

        <xsl:variable name="test_result_for_toolset" select="$test_results[ @toolset = $toolset ]"/>
        <xsl:choose>
          <xsl:when test="$mode='user'">
            <xsl:call-template name="insert_cell_user">
              <xsl:with-param name="test_log" select="$test_result_for_toolset"/>
              <xsl:with-param name="log_link" select="concat( $links_file, '#', $test_name, '-', $toolset )"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$mode='developer'">
            <xsl:call-template name="insert_cell_developer">
              <xsl:with-param name="test_log" select="$test_result_for_toolset"/>
              <xsl:with-param name="log_link" select="concat( $links_file, '#', $test_name, '-', $toolset )"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
        
      </xsl:for-each>
      <xsl:copy-of select="$test_header"/>
    </tr>
  </xsl:template>


</xsl:stylesheet>
