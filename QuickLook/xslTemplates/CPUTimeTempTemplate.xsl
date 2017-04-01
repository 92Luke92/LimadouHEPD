<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: CPUTimeTempTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the CPUTimeTempToXml.c 
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->

   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>CPU Time, CPU Temperature and PMT/Trigger Board Temperature</h2>
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

	     	     
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> CPU Time (ms) </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> Start  </td>
		   <td width="100" height="30" align="center"> Stop  </td>

		 </tr>
	       </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> CPU Temperature (°C) </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> Start  </td>
		   <td width="100" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> PMT/Trigger Board Temperature (°C) </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> Start  </td>
		   <td width="100" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>
	     
	   </tr>
	   

	   
	   <xsl:for-each select="//CPUTIMETEMP">

	     <tr>
	       <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="RUN_ID"/>         </td>
		   </tr>
		 </table>
	       </td>


	         <td>
		 <table border="1">
		   <tr>    
	              <td width="100" height="30" align="center"><xsl:value-of select="CPU_START_TIME"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="CPU_STOP_TIME"/>         </td>      
		   </tr>
		 </table>
		 </td>
		 
		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="CPU_TEMP_START_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="CPU_TEMP_START"/> </td>
		       </xsl:when>
		       <xsl:when test="CPU_TEMP_START_R &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="CPU_TEMP_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CPU_TEMP_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     
		       <xsl:choose>
		       <xsl:when test="CPU_TEMP_STOP_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="CPU_TEMP_STOP"/> </td>
		       </xsl:when>
		       <xsl:when test="CPU_TEMP_STOP_R &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="CPU_TEMP_STOP"/> </td>
		       </xsl:when>       
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CPU_TEMP_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      
    
		   </tr>
		 </table>
		 </td>

		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="PMT_TEMP_START_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="PMT_TEMP_START"/> </td>
		       </xsl:when>
		       <xsl:when test="PMT_TEMP_START_R &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="PMT_TEMP_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="PMT_TEMP_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     
		       <xsl:choose>
		       <xsl:when test="PMT_TEMP_STOP_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="PMT_TEMP_STOP"/> </td>
		       </xsl:when>
		       <xsl:when test="PMT_TEMP_STOP_R &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="PMT_TEMP_STOP"/> </td>
		       </xsl:when>       
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="PMT_TEMP_STOP"/>        </td>
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

