<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
<xsl:template match="/task">
<select>
<xsl:for-each select=".//task">
        <option name="optname" value="{@id}">
<xsl:for-each select="ancestor::task"><xsl:text> </xsl:text></xsl:for-each> <!-- or   if you needed simple space -->
<xsl:value-of select="@name"/>
        </option>
</xsl:for-each>
</select>
</xsl:template>
</xsl:stylesheet>