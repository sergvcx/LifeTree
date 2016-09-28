<?xml version='1.0'?>
<xsl:stylesheet version="1.0"
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="stock">
   <DIV STYLE="font-weight:bold">
      Symbol: <xsl:value-of select="symbol" />, 
      Price: <xsl:value-of select="price" />
   </DIV>
</xsl:template>

</xsl:stylesheet>