<?xml version='1.0'?>
<xsl:stylesheet version="1.0"
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="taskk">
   <DIV STYLE="font-weight:bold">
      name: <xsl:value-of select="@name" />, 
      time: <xsl:value-of select="@time" />
   </DIV>
   <xsl:apply-templates select="task"/>
</xsl:template>

<xsl:template match="task">
   <DIV STYLE="font-weight:bold">
      name-: <xsl:value-of select="@name" />, 
      time-: <xsl:value-of select="@time" />
   </DIV>
   <xsl:apply-templates select="task/task"/>
</xsl:template>

<xsl:template match="task/task">
   <DIV STYLE="font-weight:bold">
      name=: <xsl:value-of select="@name" />, 
      time=: <xsl:value-of select="@time" />
   </DIV>
</xsl:template>


</xsl:stylesheet>