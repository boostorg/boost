<?xml version="1.0" encoding="utf-8"?>
<!--

// Copyright (c) MetaCommunications, Inc. 2003-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)

-->

<xsl:stylesheet 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:exsl="http://exslt.org/common"
    xmlns:func="http://exslt.org/functions"
    xmlns:str="http://exslt.org/strings"
    xmlns:meta="http://www.meta-comm.com"
    extension-element-prefixes="func"
    exclude-result-prefixes="exsl func str meta"
    version="1.0">

    <xsl:variable name="output_directory" select="'output'"/>

    <func:function name="meta:test_structure">
        <xsl:param name="document"/>
        <xsl:variable name="required_toolsets" select="$explicit_markup//mark-toolset[ @status='required' ]"/>

        <xsl:variable name="runs" select="//test-run"/>
        <xsl:variable name="run_toolsets_f">
        <runs>
            <xsl:for-each select="$runs">
                <run 
                    runner="{@runner}" 
                    timestamp="{@timestamp}" 
                    platform="{@platform}"
                    run-type="{@run-type}"
                    source="{@source}">

                    <comment><xsl:value-of select="comment"/></comment>
                    <xsl:variable name="not_ordered_toolsets" select="set:distinct( .//test-log[ meta:is_test_log_a_test_case(.) ]/@toolset )"/>

                    <xsl:variable name="not_ordered_toolsets_with_info_f">
                        <xsl:for-each select="$not_ordered_toolsets">
                            <xsl:sort select="." order="ascending"/>
                            <xsl:variable name="toolset" select="."/>
                            <xsl:variable name="required">
                                <xsl:choose>
                                    <xsl:when test="count( $required_toolsets[ @name = $toolset ] ) > 0">yes</xsl:when>
                                    <xsl:otherwise>no</xsl:otherwise>
                                </xsl:choose>
                            </xsl:variable>
                            <xsl:variable name="required_sort_hint">
                                <xsl:choose>
                                    <xsl:when test="$required = 'yes'">sort hint A</xsl:when>
                                    <xsl:otherwise>sort hint B</xsl:otherwise>
                                </xsl:choose>
                            </xsl:variable>
                            <toolset name="{$toolset}" required="{$required}" required_sort_hint="{$required_sort_hint}"/>
                        </xsl:for-each>
                    </xsl:variable>

                    <xsl:variable name="not_ordered_toolsets_with_info" select="exsl:node-set( $not_ordered_toolsets_with_info_f )"/>
                    
                    <xsl:for-each select="$not_ordered_toolsets_with_info/toolset">
                        <xsl:sort select="concat( @required_sort_hint, '-', @name )" order="ascending"/>
                        <xsl:copy-of select="."/>
                    </xsl:for-each>
                </run>
            </xsl:for-each>
        </runs>
        </xsl:variable>

        <func:result select="exsl:node-set( $run_toolsets_f )"/>
    </func:function>

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

    <func:function name="meta:show_output">
        <xsl:param name="explicit_markup"/>     
        <xsl:param name="test_log"/>     
        <func:result select="$test_log/@result != 'success' and not( meta:is_unusable( $explicit_markup, $test_log/@library, $test_log/@toolset )) or $test_log/@show-run-output = 'true'"/>
    </func:function>

    <func:function name="meta:is_test_log_a_test_case">
        <xsl:param name="test_log"/>      
        <func:result select="$test_log/@test-type='compile' or $test_log/@test-type='compile_fail' or $test_log/@test-type='run' or $test_log/@test-type='run_pyd'"/>
    </func:function>

    <func:function name="meta:is_unusable">
        <xsl:param name="explicit_markup"/>
        <xsl:param name="library"/>
        <xsl:param name="toolset"/>
          
        <func:result select="$explicit_markup//library[ @name = $library ]/mark-unusable[ toolset/@name = $toolset or toolset/@name='*' ]"/>
    </func:function>

    <func:function name="meta:encode_path">
        <xsl:param name="path"/>
        <func:result select="translate( translate( $path, '/', '-' ), './', '-' )"/>
    </func:function>

    <func:function name="meta:output_file_path">
        <xsl:param name="path"/>
        <func:result select="concat( $output_directory, '/', meta:encode_path( $path ), '.html' )"/>
    </func:function>

    <xsl:template name="insert_runners_rows">
        <xsl:param name="mode"/>
        
        <xsl:variable name="colspan">
            <xsl:choose>
                <xsl:when test="$mode = 'summary'">1</xsl:when>
                <xsl:when test="$mode = 'details'">2</xsl:when>
            </xsl:choose>
        </xsl:variable>
        
        <tr>
            <td colspan="{$colspan}">&#160;</td>
            <xsl:for-each select="$run_toolsets/runs/run">
                <td colspan="{count(toolset)}" class="runner">
                    <a href="../{@runner}.html">
                        <xsl:value-of select="@runner"/>
                    </a>
                    <br/>
                    <xsl:value-of select="@platform"/>
                </td>
            </xsl:for-each>
            <td colspan="{$colspan}">&#160;</td>
        </tr>

        <tr>
            <td colspan="{$colspan}">&#160;</td>
            <xsl:for-each select="$run_toolsets/runs/run">
                <td colspan="{count(toolset)}" class="timestamp"><xsl:value-of select="@timestamp"/></td>
            </xsl:for-each>
            <td colspan="{$colspan}">&#160;</td>
        </tr>

    </xsl:template>

    <xsl:template name="insert_toolsets_row">
        <xsl:param name="mode"/>
        <xsl:param name="library"/>
        <xsl:param name="library_marks"/>

        <tr valign="middle">
            <xsl:variable name="colspan">
                <xsl:choose>
                    <xsl:when test="$mode = 'summary'">1</xsl:when>
                    <xsl:when test="$mode = 'details'">2</xsl:when>
                </xsl:choose>
            </xsl:variable>

            <xsl:variable name="title">
                <xsl:choose>
                    <xsl:when test="$mode = 'summary'">&#160;library&#160;/&#160;toolset&#160;</xsl:when>
                    <xsl:when test="$mode = 'details'">&#160;test&#160;/&#160;toolset&#160;</xsl:when>
                </xsl:choose>
            </xsl:variable>
              
            <td class="head" colspan="{$colspan}" width="1%"><xsl:value-of select="$title"/></td>
              

            <xsl:for-each select="$run_toolsets/runs/run/toolset">
                <xsl:variable name="toolset" select="@name"/>
                  
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

                <td class="{$class}">

                    <xsl:for-each select="str:tokenize($toolset, '-')">
                        <xsl:value-of select="." />
                        <xsl:if test="position()!=last()">
                            <xsl:text>- </xsl:text>
                        </xsl:if>
                    </xsl:for-each>
                    
                    <xsl:if test="$mode = 'details'">
                        <xsl:variable name="toolset_notes_fragment">
                            <xsl:for-each select="$library_marks/note">
                                <xsl:if test="../@toolset=$toolset or ( ../toolset/@name=$toolset or ../toolset/@name = '*' )">
                                    <note index="{position()}"/>
                                </xsl:if>
                            </xsl:for-each>
                        </xsl:variable>
                        
                        <xsl:variable name="toolset_notes" select="exsl:node-set( $toolset_notes_fragment )/*"/>
                        <xsl:if test="count( $toolset_notes ) > 0">
                            <span class="super">
                            <xsl:for-each select="$toolset_notes">
                                <xsl:variable name="note_index" select="@index"/>
                                <xsl:if test="generate-id( . ) != generate-id( $toolset_notes[1] )">, </xsl:if>
                                <a href="#{$library}-note-{$note_index}" title="Note {$note_index}">
                                    <xsl:value-of select="$note_index"/>
                                </a>
                            </xsl:for-each>
                            </span>
                        </xsl:if>
                    </xsl:if>
                </td>
            </xsl:for-each>
              
        <td class="head" width="1%"><xsl:value-of select="$title"/></td>
    </tr>
    </xsl:template>

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
