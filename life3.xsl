<?xml version="1.0" encoding="WINDOWS-1251" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/TR/WD-xsl">
<xsl:template match="/">
<table border="1">
<tr bgcolor="#CCCCCC">
<td align="center"><strong>name</strong></td>
<td align="center"><strong>time</strong></td>
<td align="center"><strong>cost</strong></td>
</tr>
<xsl:for-each select="task/task">
<tr bgcolor="#F5F5F5">
<td><xsl:value-of select="@name"/></td>
<td align="right"><xsl:value-of select="@time"/></td>
<td><xsl:value-of select="@cost"/></td>
</tr>
</xsl:for-each>
</table>
</xsl:template>
</xsl:stylesheet>