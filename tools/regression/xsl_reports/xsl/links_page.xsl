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

  <xsl:param name="source"/>
  <xsl:param name="run_date"/>
  <xsl:param name="comment_file"/>
  <xsl:param name="expected_results_file"/>

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
              <td><img border="0" src="../c++boost.gif" width="277" height="86" alt="Boost logo"></img></td>
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
          <xsl:apply-templates select="//test-log[@result = 'fail']"/>
        </div>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="test-log">
    <div>
      <xsl:variable name="test-anchor">
        <xsl:value-of select="concat( @test-name, '-', @toolset )"/>
      </xsl:variable>
      <div class="log-test-title">
        <a name="{$test-anchor}"><xsl:value-of select="concat( @test-name, ' / ', @toolset )"/></a>
      </div>

    <xsl:if test="notes/note">
      <p>
        <div class="log-notes-title">Notes:</div>
        <div class="log-notes">
          <xsl:for-each select="notes/note">
            <div>
              <xsl:if test="@date">
                <xsl:value-of select="@date"/><xsl:text> </xsl:text>
              </xsl:if>
              <xsl:if test="@author">
                <xsl:value-of select="@author"/><xsl:text> </xsl:text>
              </xsl:if>
              <xsl:copy-of select="node()"/>
            </div>
          </xsl:for-each>
        </div>
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
        <pre>
          <xsl:copy-of select="lib/node()"/>
        </pre>
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
</xsl:stylesheet>
