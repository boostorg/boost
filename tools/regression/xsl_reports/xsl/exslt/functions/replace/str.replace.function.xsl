<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                xmlns:func="http://exslt.org/functions"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="str exsl func">

<func:function name="str:replace">
	<xsl:param name="string" select="''" />
   <xsl:param name="search" select="/.." />
   <xsl:param name="replace" select="/.." />
   <xsl:choose>
      <xsl:when test="not($string)">
        <func:result select="/.." />
      </xsl:when>
      <xsl:when test="function-available('exsl:node-set')">
         <!-- this converts the search and replace arguments to node sets
              if they are one of the other XPath types -->
         <xsl:variable name="search-nodes-rtf">
           <xsl:copy-of select="$search" />
         </xsl:variable>
         <xsl:variable name="replace-nodes-rtf">
           <xsl:copy-of select="$replace" />
         </xsl:variable>
         <xsl:variable name="replacements-rtf">
            <xsl:for-each select="exsl:node-set($search-nodes-rtf)/node()">
               <xsl:variable name="pos" select="position()" />
               <replace search="{.}">
                  <xsl:copy-of select="exsl:node-set($replace-nodes-rtf)/node()[$pos]" />
               </replace>
            </xsl:for-each>
         </xsl:variable>
         <xsl:variable name="sorted-replacements-rtf">
            <xsl:for-each select="exsl:node-set($replacements-rtf)/replace">
               <xsl:sort select="string-length(@search)" data-type="number" order="descending" />
               <xsl:copy-of select="." />
            </xsl:for-each>
         </xsl:variable>
         <xsl:variable name="result">
           <xsl:choose>
              <xsl:when test="not($search)">
                <xsl:value-of select="$string" />
              </xsl:when>
             <xsl:otherwise>
               <xsl:call-template name="str:_replace">
                  <xsl:with-param name="string" select="$string" />
                  <xsl:with-param name="replacements" select="exsl:node-set($sorted-replacements-rtf)/replace" />
               </xsl:call-template>
             </xsl:otherwise>
           </xsl:choose>
         </xsl:variable>
         <func:result select="exsl:node-set($result)/node()" />
      </xsl:when>
      <xsl:otherwise>
         <xsl:message terminate="yes">
            ERROR: function implementation of str:replace() relies on exsl:node-set().
         </xsl:message>
      </xsl:otherwise>
   </xsl:choose>
</func:function>

<xsl:template name="str:_replace">
  <xsl:param name="string" select="''" />
  <xsl:param name="replacements" select="/.." />
  <xsl:choose>
    <xsl:when test="not($string)" />
    <xsl:when test="not($replacements)">
      <xsl:value-of select="$string" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:variable name="replacement" select="$replacements[1]" />
      <xsl:variable name="search" select="$replacement/@search" />
      <xsl:choose>
        <xsl:when test="not(string($search))">
          <xsl:value-of select="substring($string, 1, 1)" />
          <xsl:copy-of select="$replacement/node()" />
          <xsl:call-template name="str:_replace">
            <xsl:with-param name="string" select="substring($string, 2)" />
            <xsl:with-param name="replacements" select="$replacements" />
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="contains($string, $search)">
          <xsl:call-template name="str:_replace">
            <xsl:with-param name="string" select="substring-before($string, $search)" />
            <xsl:with-param name="replacements" select="$replacements[position() > 1]" />
          </xsl:call-template>      
          <xsl:copy-of select="$replacement/node()" />
          <xsl:call-template name="str:_replace">
            <xsl:with-param name="string" select="substring-after($string, $search)" />
            <xsl:with-param name="replacements" select="$replacements" />
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="str:_replace">
            <xsl:with-param name="string" select="$string" />
            <xsl:with-param name="replacements" select="$replacements[position() > 1]" />
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>