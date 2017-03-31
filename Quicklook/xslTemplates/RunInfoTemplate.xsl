<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: RunInfoTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the RunInfoToXml.c 
=====================================================================================
-->
    

<!-- <xsl:include href="numberutils_lib.xsl"/> -->

   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>Run Information </h2>
	 <table border="1">
	   <tr bgcolor="#1acd32">
	     <td>
	     <table border="1">
	       <tr  bgcolor="#1acd32">
	         <td height="30" align="center" colspan="2"> Run ID </td>
	       </tr>
	       <tr>
	         <td width="100" height="30" align="center">Boot Number</td>
	         <td width="100" height="30" align="center">Run Number</td>
	       </tr>
	     </table>
	     </td>
	     
	     
	     <td width="150" height="30" align="center">Run Type</td>
	     <td width="150" height="30" align="center">Run Duration (s)</td>
	     <td width="150" height="30" align="center">Orbit Zone</td>
	   </tr>
	   
	   <xsl:for-each select="//RUN_INFO">
	     <tr>
	       <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="60" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="100" height="60" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		   </tr>
		 </table>
	       </td>

	       <xsl:choose>
	       <xsl:when test="RUN_TYPE_error &gt;'0'">
	        <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="RUN_TYPE"/> </td>
	       </xsl:when>
	       <xsl:otherwise>
		 <td width="150" height="30" align="center"><xsl:value-of select="RUN_TYPE"/>        </td>
	       </xsl:otherwise>
	       </xsl:choose>
	       
	       <xsl:choose>
	       <xsl:when test="RUN_DURATION_error &gt;'0'">
	        <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="RUN_DURATION"/> </td>
	       </xsl:when>
	       <xsl:otherwise>
		 <td width="150" height="30" align="center"><xsl:value-of select="RUN_DURATION"/>        </td>
	       </xsl:otherwise>
	       </xsl:choose>	       

	        <xsl:choose>
	       <xsl:when test="ORBIT_error &gt;'0'">
	        <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="ORBIT_ZONE"/> </td>
	       </xsl:when>
	       <xsl:otherwise>
		 <td width="150" height="30" align="center"><xsl:value-of select="ORBIT_ZONE"/>        </td>
	       </xsl:otherwise>
	       </xsl:choose>
	      
	       
	     </tr>	

	  </xsl:for-each>
  	 </table>
       </body>
     </html>
   </xsl:template>
 
 </xsl:stylesheet>
