<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: SilConfigTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the SilConfigToXml.c 
=====================================================================================
-->
    

<!-- <xsl:include href="numberutils_lib.xsl"/> -->


   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>Silicon Configuration </h2>
	 <table border="1">
	   <tr bgcolor="#1acd32">
	     <td>
	     <table border="1">
	       <tr  bgcolor="#1acd32">
	         <td height="50" align="center" colspan="2"> Run ID </td>
	       </tr>
	       <tr>
	         <td width="100" height="100" align="center">Boot Number</td>
	         <td width="100" height="100" align="center">Run Number</td>
	       </tr>
	     </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="50" align="center" colspan="2">Ladder Masks</td>
		 </tr>
		 <tr>
		   <td width="100" height="100" align="center"> Current SD Ladders Mask </td>
		   <td width="100" height="100" align="center"> Ladders LV ON/OFF Mask </td> 
		 </tr>
	       </table>
	     </td>
	     
	     <td>
	       <table border="1">
		 <tr bgcolor="#1acd32">
		   <td height="50" align="center" colspan="9"> SD Parameters </td>
		 </tr>
		 <tr  bgcolor="#1acd32">
		   <td width="100" height="100" align="center">Adiacent Strip Zero-Supp. Algorithms</td>
		   <td width="100" height="100" align="center">Threshold Zero-Supp. Algorithms</td>
		   <td width="100" height="100" align="center">Threshold Common Noise High</td>
		   <td width="100" height="100" align="center">Threshold Common Noise Low</td>
		   <td width="100" height="100" align="center">Calibration Events for RMS</td>
		   <td width="100" height="100" align="center">Calibration Events for Pedestal</td>
		   <td width="100" height="100" align="center">Calibration Events for Gaussian Check</td>
		   <td width="100" height="100" align="center">Calibration Events for Common Noise Calculation</td>
		   <td width="100" height="100" align="center">Parameter for Gaussian Check</td> 
		 </tr>
	       </table>
	     </td>
	   </tr>

	   
	   <xsl:for-each select="//SILCONFIG">

	     <tr>
	       <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="60" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="100" height="60" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		   </tr>
		 </table>
	       </td>

	       <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="LADDER_MASK_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="CURRENT_SD"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="CURRENT_SD"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="LADDER_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="LADDERS_LV"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="LADDERS_LV"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		    
                   </tr>
		 </table>
	       </td>
	    
	       <td>
		  <table border="1">
		    <tr>

		      <xsl:choose>
		       <xsl:when test="ADJ_STRIP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="ADIACENT_STRIP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="ADIACENT_STRIP"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="ZERO_SUPP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="ZERO_SUPP_THRD"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="ZERO_SUPP_THRD"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		     <xsl:choose>
		       <xsl:when test="CN_HIGH_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="THRD_CN_HIGH"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="THRD_CN_HIGH"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="CN_LOW_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="THRD_CN_LOW"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="THRD_CN_LOW"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		        <xsl:choose>
		       <xsl:when test="EVENT_RMS_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="CALIB_EVENT_RMS"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="CALIB_EVENT_RMS"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="EVENT_PED_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="CALIB_EVENT_PED"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="CALIB_EVENT_PED"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="EVENT_GAUSS_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="CALIB_EVENT_GAUSS"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="CALIB_EVENT_GAUSS"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		       
		      <xsl:choose>
		       <xsl:when test="EVENT_CN_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="CALIB_EVENT_CN"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="CALIB_EVENT_CN"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="GAUSS_CHECK_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="60" align="center"> <xsl:value-of select="GAUSS_CHECK"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="60" align="center"><xsl:value-of select="GAUSS_CHECK"/>        </td>
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
