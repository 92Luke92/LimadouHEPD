<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: GPSBroadcastTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the BroadcastToXml.c 
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->
   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>GPS and Velocity Broadcast</h2>
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

	     <td  width="100" height="30" align="center"> OBDH Time Broadcast (ms) </td>
	     <td  width="10" height="30" align="center"> Timestamp GPS Broadcast (ms) </td>
	       
	     
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="3"> GPS Position (m) </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> X axis </td>
		   <td width="100" height="30" align="center"> Y axis  </td>
		   <td width="100" height="30" align="center"> z axis  </td>    

		 </tr>
	       </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="3"> Velocity (m/s) </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center">  X axis  </td>
		   <td width="100" height="30" align="center">  Y axis   </td>
		   <td width="100" height="30" align="center">  Z axis   </td>    

		 </tr>
	       </table>
	     </td>

	      <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> Sub-Satellite Point </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> Longitude  </td>
		   <td width="100" height="30" align="center"> Latitude   </td>
		 </tr>
	       </table>
	      </td>
	      
	   </tr>

	   
	   <xsl:for-each select="//BROADCAST2">

	     <tr>
	       <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		   </tr>
		 </table>
	       </td>

	       <td width="100" height="30" align="center"><xsl:value-of select="OBDH_MS"/>        </td>
	       <td width="100" height="30" align="center"><xsl:value-of select="TIMESTAMP_GPS"/>        </td>

	         <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="30" align="center"><xsl:value-of select="GPS_POS_X"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="GPS_POS_Y"/>         </td>
		      <td width="100" height="30" align="center"><xsl:value-of select="GPS_POS_Z"/>         </td>
		      
		   </tr>
		 </table>
		 </td>
		 

		 <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="30" align="center"><xsl:value-of select="GPS_VEL_X"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="GPS_VEL_Y"/>         </td>
		      <td width="100" height="30" align="center"><xsl:value-of select="GPS_VEL_Z"/>         </td>
		      
		   </tr>
		 </table>
	       </td>

	       <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="LONG_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="LONGITUDE"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="LONGITUDE"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="LAT_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="LATITUDE"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="LATITUDE"/>        </td>
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
