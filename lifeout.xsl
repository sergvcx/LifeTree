<?xml version='1.0' encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform">




<xsl:template match="/task">
<style type="text/css">
	ul { 
		display: block;
		list-style-type: disc;
		margin-top:0 em;
		margin-bottom: 0 em;
		margin-left: 1 em;
		margin-right: 0;
		padding-left: 10px;
	}
	</style>

   <ul>
   <li>
      <xsl:value-of select="@name" /> 
      <ul>
	  <xsl:apply-templates select="task"/>
	  </ul>
   </li>
   </ul>
</xsl:template>

<xsl:template match="task">
   <li>
      <xsl:value-of select="@name" /> 
      <ul>
	  <xsl:apply-templates select="task"/>
	  </ul>
   </li>
</xsl:template>





</xsl:stylesheet>