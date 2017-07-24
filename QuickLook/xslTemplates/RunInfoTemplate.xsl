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
	 <h5>We defined 5 orbit zones: equatorial, south polar, north polar, SAA (South Atlantic Anomaly) and default zone (when no orbit information is available).</h5>
	 <h5>By standby zone we do not mean one of the above-mentioned orbit zones, but we mean that HEPD is not in data acquisition. </h5>
	 <table border="1">
	   <tr bgcolor="#1acd32">

	     <td>
	       <tr  bgcolor="#1acd32">
	         <td></td>
	       </tr>
	     </td>

	     <td>
	     <table border="1">
	       <tr  bgcolor="#1acd32">
	         <td height="30" align="center" colspan="2"> Run ID </td>
	       </tr>
	       <tr>
	         <td width="60" height="30" align="center">Boot nr.</td>
	         <td width="60" height="30" align="center">Run nr.</td>
	       </tr>
	     </table>
	     </td>
	     
	     
	     <td width="150" height="30" align="center">Run Type</td>
	     <td width="150" height="30" align="center">Configured Run Duration (s)</td>

	     <td>
	     <table border="1">
	       <tr  bgcolor="#1acd32">
	         <td height="30" align="center" colspan="2"> Orbit Zone </td>
	       </tr>
	       <tr>
	         <td width="150" height="30" align="center">Calculated</td>
	         <td width="150" height="30" align="center">Applied</td>
	       </tr>
	     </table>
	     </td>
	     
	   </tr>
	   
	   <xsl:for-each select="//RUN_INFO">
	     <tr>
	       <td>
		   <tr>     
		     <xsl:choose>
		       <xsl:when test="HEADER &gt;'0'">
			 <td  width="20" height="30" align="center"> <xsl:value-of select="HEADER_VAL"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="20" height="30" align="center"><xsl:value-of select="TAIL_VAL"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>	     
		   </tr>
	       </td>

	       
	       <td>
		 <table border="1">
		   <tr>
	              <td width="60" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="60" height="30" align="center"><xsl:value-of select="RUN_NR"/>         </td>
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

	       <td>
		 <table border="1">	
		   <tr>


	       <xsl:choose>
	       <xsl:when test="ORBIT_error &gt;'0'">
	        <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="ORBIT_ZONE_Calculated"/> </td>
	       </xsl:when>
	        <xsl:when test="ORBIT_error_yellow &gt;'0'">
	        <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="ORBIT_ZONE_Calculated"/> </td>
	       </xsl:when>
	       <xsl:otherwise>
		 <td width="150" height="30" align="center"><xsl:value-of select="ORBIT_ZONE_Calculated"/>        </td>
	       </xsl:otherwise>
		</xsl:choose>


	       <xsl:choose>
	       <xsl:when test="ORBIT_error &gt;'0'">
	        <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="ORBIT_ZONE_Applied"/> </td>
	       </xsl:when>
	        <xsl:when test="ORBIT_error_yellow &gt;'0'">
	        <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="ORBIT_ZONE_Applied"/> </td>
	       </xsl:when>
	       <xsl:otherwise>
		 <td width="150" height="30" align="center"><xsl:value-of select="ORBIT_ZONE_Applied"/>        </td>
	       </xsl:otherwise>
		</xsl:choose>

		   </tr>
		   </table>
		 </td>









	       
	     </tr>	

	  </xsl:for-each>
  	 </table>
       </body>
     </html>
   </xsl:template>
 
 </xsl:stylesheet>
