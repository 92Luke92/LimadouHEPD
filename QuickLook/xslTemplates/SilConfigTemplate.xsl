<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: SilConfigTemplate.xsl
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
	       <tr  bgcolor="#1acd32">
	         <td></td>
	       </tr>
	     </td>

	     <td>
	     <table border="1">
	       <tr  bgcolor="#1acd32">
	         <td height="100" align="center" colspan="2"> Run ID </td>
	       </tr>
	       <tr>
	         <td width="60" height="50" align="center">Boot nr.</td>
	         <td width="60" height="50" align="center">Run nr.</td>
	       </tr>
	     </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="50" align="center" colspan="2">Ladder Masks </td>
		 </tr>
		 <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td width="300" height="50" align="center" colspan="3"> Current SD Ladders Mask </td>
		   <td width="300" height="50" align="center" colspan="3"> Ladders LV ON/OFF Mask </td>
		 </tr>
		   <tr>
		     <td width="50" height="50" align="center"> Top </td>
		     <td width="70" height="50" align="center"> Centr. </td>
		     <td width="60" height="50" align="center"> Bott. </td>
		     <td width="50" height="50" align="center"> Top </td>
		     <td width="70" height="50" align="center"> Centr. </td>
		     <td width="60" height="50" align="center"> Bott. </td>
	
		  </tr>
	
		 </table>	
		 
	       </table>
	     </td>
	     
	     <td>
	       <table border="1">
		 <tr bgcolor="#1acd32">
		   <td height="50" align="center" colspan="9"> SD Parameters </td>
		 </tr>
		 <tr  bgcolor="#1acd32">
		   <td width="100" height="100" align="center">Adiacent Strip Zero-Supp.</td>
		   <td width="100" height="100" align="center">Threshold Zero-Supp.</td>
		   <td width="100" height="100" align="center">Threshold Com. Noise High</td>
		   <td width="100" height="100" align="center">Threshold Com. Noise Low</td>
		   <td width="100" height="100" align="center">Calib. Events RMS</td>
		   <td width="100" height="100" align="center">Calib. Events Pedestal</td>
		   <td width="100" height="100" align="center">Calib. Events Gaussian Check</td>
		   <td width="100" height="100" align="center">Calib. Events Com. Noise Calculation</td>
		   <td width="100" height="100" align="center">Parameter Gaussian Check</td> 
		 </tr>
	       </table>
	     </td>
	   </tr>

	   
	   <xsl:for-each select="//SILCONFIG">

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

	       <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="LADDER_MASK_error_top &gt;'0'">
			 <td  bgcolor="#ffff00" width="50" height="30" align="center"> <xsl:value-of select="CURRENT_SD_top"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="50" height="30" align="center"><xsl:value-of select="CURRENT_SD_top"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="LADDER_MASK_error_central &gt;'0'">
			 <td  bgcolor="#ffff00" width="70" height="30" align="center"> <xsl:value-of select="CURRENT_SD_central"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="70" height="30" align="center"><xsl:value-of select="CURRENT_SD_central"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="LADDER_MASK_error_bottom &gt;'0'">
			 <td  bgcolor="#ffff00" width="60" height="30" align="center"> <xsl:value-of select="CURRENT_SD_bottom"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="60" height="30" align="center"><xsl:value-of select="CURRENT_SD_bottom"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="LADDER_error_top &gt;'0'">
			 <td  bgcolor="#ffff00" width="50" height="30" align="center"> <xsl:value-of select="LADDERS_LV_top"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="50" height="30" align="center"><xsl:value-of select="LADDERS_LV_top"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>

		       <xsl:choose>
		       <xsl:when test="LADDER_error_central &gt;'0'">
			 <td  bgcolor="#ffff00" width="70" height="30" align="center"> <xsl:value-of select="LADDERS_LV_central"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="70" height="30" align="center"><xsl:value-of select="LADDERS_LV_central"/>        </td>
		       </xsl:otherwise>
		       </xsl:choose>

		       <xsl:choose>
		       <xsl:when test="LADDER_error_bottom &gt;'0'">
			 <td  bgcolor="#ffff00" width="60" height="30" align="center"> <xsl:value-of select="LADDERS_LV_bottom"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="60" height="30" align="center"><xsl:value-of select="LADDERS_LV_bottom"/>        </td>
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
			 <td  bgcolor="#ffff00" width="120" height="30" align="center"> <xsl:value-of select="ADIACENT_STRIP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="120" height="30" align="center"><xsl:value-of select="ADIACENT_STRIP"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="ZERO_SUPP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="120" height="30" align="center"> <xsl:value-of select="ZERO_SUPP_THRD"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="120" height="30" align="center"><xsl:value-of select="ZERO_SUPP_THRD"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		     <xsl:choose>
		       <xsl:when test="CN_HIGH_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="110" height="30" align="center"> <xsl:value-of select="THRD_CN_HIGH"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="110" height="30" align="center"><xsl:value-of select="THRD_CN_HIGH"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="CN_LOW_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="110" height="30" align="center"> <xsl:value-of select="THRD_CN_LOW"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="110" height="30" align="center"><xsl:value-of select="THRD_CN_LOW"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		        <xsl:choose>
		       <xsl:when test="EVENT_RMS_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="70" height="30" align="center"> <xsl:value-of select="CALIB_EVENT_RMS"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="70" height="30" align="center"><xsl:value-of select="CALIB_EVENT_RMS"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="EVENT_PED_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="90" height="30" align="center"> <xsl:value-of select="CALIB_EVENT_PED"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="90" height="30" align="center"><xsl:value-of select="CALIB_EVENT_PED"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="EVENT_GAUSS_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="CALIB_EVENT_GAUSS"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CALIB_EVENT_GAUSS"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		       
		      <xsl:choose>
		       <xsl:when test="EVENT_CN_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="CALIB_EVENT_CN"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CALIB_EVENT_CN"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="GAUSS_CHECK_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="GAUSS_CHECK"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="GAUSS_CHECK"/>        </td>
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
