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
  xmlns:func="http://exslt.org/functions"
  xmlns:meta="http://www.meta-comm.com"
  extension-element-prefixes="func"
  version="1.0">

  <xsl:import href="common.xsl"/>

  <xsl:output method="html" 
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" 
    encoding="utf-8" 
    indent="yes"
    />



  <xsl:param name="source"/>
  <xsl:param name="run_date"/>
  <xsl:param name="comment_file"/>
  <xsl:param name="explicit_markup_file"/>

  <xsl:variable name="explicit_markup" select="document( $explicit_markup_file )"/>

  <xsl:template match="/">
    <html>
      <head>
        <link rel="stylesheet" type="text/css" href="master.css" title="master" />
        <title>Boost regression - test run output: <xsl:value-of select="$source"/></title>
      </head>
      <body>
        <div>
          <table border="0">
            <tr>
              <td><img border="0" src="../../c++boost.gif" width="277" height="86" alt="Boost logo"></img></td>
              <td>
                <h1 class="page-title">
                  <xsl:text>Regression Tests Log: </xsl:text>
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
                Provides notes,  compiler, linker and run output of the
                regression tests. 
              </td>
            </tr>
          </table>
        </div>
        <div>
          <xsl:apply-templates select="//test-log[@test-name != '' and @result = 'fail']"/>
        </div>
        <div>
          <xsl:apply-templates select="//test-log[@test-name = '' and @result = 'fail']" />
        </div>

      </body>
    </html>
  </xsl:template>


  <xsl:template match="test-log">
    <div>
      <xsl:choose>
        <xsl:when test="@test-name != ''">
          <xsl:variable name="test-anchor">
            <xsl:value-of select="concat( @library, '-', @test-name, '-', @toolset )"/>
          </xsl:variable>
          <div class="log-test-title">
            <a name="{$test-anchor}"><xsl:value-of select="concat( @library, ' - ', @test-name, ' / ', @toolset )"/></a>
          </div>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="test-anchor">
            <xsl:value-of select="meta:path-to-anchor( @target-directory )"/>
          </xsl:variable>
          <div class="log-test-title">
            <a name="{$test-anchor}"><xsl:value-of select="@target-directory"/></a>
          </div>
        </xsl:otherwise>
      </xsl:choose>

    <xsl:if test="notes/note">
      <p>
        <div class="notes-title">Notes</div>
        <xsl:call-template name="show_notes">
            <xsl:with-param name="notes" select="notes/note"/>
            <xsl:with-param name="explicit_markup" select="$explicit_markup"/>
        </xsl:call-template>
      </p>
    </xsl:if>

    <xsl:if test="compile">
      <p>
        <div class="log-compiler-output-title">Compiler&#160;output:</div>
        <pre>
          <xsl:copy-of select="compile/node()"/>
        </pre>
      </p>
    </xsl:if>

    <xsl:if test="link">
      <p>
        <div class="log-linker-output-title">Linker&#160;output:</div>
        <pre>
          <xsl:copy-of select="link/node()"/>
        </pre>
      </p>
    </xsl:if>

    <xsl:if test="lib">
      <p>
        <div class="log-linker-output-title">Lib &#160;output:</div>
        <p>
          See <a href="#{meta:path-to-anchor( lib/node() )}">
            <xsl:copy-of select="lib/node()"/>
          </a>
        </p>
      </p>
    </xsl:if>

    <xsl:if test="run">
      <p>
        <div class="log-run-output-title">Run&#160;output:</div>
        <pre>
          <xsl:copy-of select="run/node()"/>
        </pre>
      </p>
    </xsl:if>

    </div>

  </xsl:template>

  <func:function name="meta:path-to-anchor">
      <xsl:param name="path"/>
      <func:result select="translate( $path, '/', '-' )"/>
  </func:function>
  

</xsl:stylesheet>
