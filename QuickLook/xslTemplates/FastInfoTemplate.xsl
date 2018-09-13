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
	 <h2>Fast Info </h2>
	 <h5>We defined 5 orbit zones: equatorial, south polar, north polar, SAA (South Atlantic Anomaly) and default zone (when no orbit information is available).</h5>
	 <h5>By standby zone we do not mean one of the above-mentioned orbit zones, but we mean that HEPD is not in data acquisition. </h5>
	
	     
	 <table border="1">
	   <tr bgcolor="#1acd32">
	         	    
	     <!-- <td> -->
	     <!-- <table border="1"> -->
	     <!--   <tr  bgcolor="#1acd32"> -->
	     <!--     <td height="30" align="center" colspan="2"> Run ID </td> -->
	     <!--   </tr> -->
	     <!--   <tr> -->
	     <!--     <td width="60" height="30" align="center">Boot nr.</td> -->
	     <!--     <td width="60" height="30" align="center">Run nr.</td> -->
	     <!--   </tr> -->
	     <!-- </table> -->
	     <!-- </td> -->

	       <td>
	       <tr  bgcolor="#1acd32">
	       </tr>
	       <tr>
	         <td width="60" height="30" align="center">Boot nr.</td>
	         <td width="60" height="30" align="center">Run nr.</td>
	       </tr>
	       </td>

	     
	     <td width="150" height="30" align="center">Run Type</td>
	     <td width="100" height="30" align="center">Events </td>
	     <td width="150" height="30" align="center">Run Duration (s)</td>
	     <td width="150" height="30" align="center">Applied Orbit Zone </td>
	     <td width="160" height="30" align="center">Latitude [Start; Stop] </td>
	     <td width="180" height="30" align="center">Longitude [Start; Stop] </td>
	     <td width="250" height="30" align="center">Trigger Mask </td>
	     <td width="250" height="30" align="center">Trigger Mask and PMT Mask </td>
	     <td width="80" height="30" align="center">Veto </td>


	   </tr>
	   

	    <xsl:for-each select="//RUN_INFO2">
	     <tr>
	       	       
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
	       

	       <td width="100" height="30" align="center"><xsl:value-of select="N_EVENTS"/>        </td>
	       <td width="150" height="30" align="center"><xsl:value-of select="RUN_DURATION"/>        </td>
		 
		 <xsl:choose>
		 <xsl:when test="ORBIT_error &gt;'0'">
	           <td  bgcolor="#ff0000" width="150" height="30" align="center"> <xsl:value-of select="Applied_ORBIT_ZONE"/> </td>
		 </xsl:when>
		 <xsl:when test="ORBIT_error_yellow &gt;'0'">
	          <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="Applied_ORBIT_ZONE"/> </td>
		 </xsl:when>
		 <xsl:otherwise>
		   <td width="150" height="30" align="center"><xsl:value-of select="Applied_ORBIT_ZONE"/>        </td>
		 </xsl:otherwise>
		 </xsl:choose>

		<xsl:choose>
		  <xsl:when test="LAT_error &gt;'0'">
		    <td  bgcolor="#ff0000" width="160" height="30" align="center"> <xsl:value-of select="LATITUDE"/> </td>
		  </xsl:when>
		  <xsl:otherwise>
		    <td width="160" height="30" align="center"><xsl:value-of select="LATITUDE"/>        </td>
		  </xsl:otherwise>
		</xsl:choose>

		<xsl:choose>
		  <xsl:when test="LONG_error &gt;'0'">
		  <td  bgcolor="#ff0000" width="180" height="30" align="center"> <xsl:value-of select="LONGITUDE"/> </td>
		</xsl:when>
		<xsl:otherwise>
		  <td width="180" height="30" align="center"><xsl:value-of select="LONGITUDE"/>        </td>
		</xsl:otherwise>
		</xsl:choose>


		<td width="250" height="30" align="center"><xsl:value-of select="TRIGGER"/>        </td>
		<td width="250" height="30" align="center"><xsl:value-of select="TRIGGER_PMT_MASK"/>        </td>
		<td width="80" height="30" align="center"><xsl:value-of select="VETO"/>        </td>
		
	     </tr>	

	     
	 
	    </xsl:for-each>
	     </table>
  	
       </body>
     </html>
   </xsl:template>
 
 </xsl:stylesheet>
