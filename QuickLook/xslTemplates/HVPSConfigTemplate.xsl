<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================


=====================================================================================
-->

   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>HVPS Configuration </h2>
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
	         <td width="60" height="60" align="center">Boot nr.</td>
	         <td width="60" height="60" align="center">Run nr.</td>
	       </tr>
	     </table>
	     </td>
	       
	     <td width="100" height="30" align="center">   </td>
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="10">PMT HVPS ID</td>
		 </tr>
		 <tr>
		   <td width="80" height="60" align="center"> 0 </td>
		   <td width="80" height="60" align="center"> 1  </td>
		   <td width="80" height="60" align="center"> 2  </td>    
		   <td width="80" height="60" align="center"> 3  </td>   
		   <td width="80"  height="60" align="center"> 4  </td>    
		   <td width="80" height="60" align="center"> 5  </td> 
		   <td width="80" height="60" align="center"> 6  </td>  
		   <td width="80" height="60" align="center"> 7 </td>  
		   <td width="80" height="60" align="center"> 8 </td> 
		   <td width="80" height="60" align="center"> 9 </td>  
		 </tr>
	       </table>
	     </td>
	     <td>
	       <table border="1">
		 <tr bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> Silicon Plane </td>
		 </tr>
		 <tr  bgcolor="#1acd32">
		   <td width="100" height="60" align="center">A</td>
		   <td width="100" height="60" align="center">B</td>
		 </tr>
	       </table>
	     </td>
	   </tr>

	   
	   <xsl:for-each select="//HVPSCONFIG">

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
	              <td width="60" height="70" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="60" height="70" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		   </tr>
		 </table>
	       </td>

	       
	       <td>
		 <table border="1">
		   <tr>
		     <td rowspan="2"> </td>	  

 	 	       <td  width="100" height="30" align="center"> ON/OFF Mask </td>
		   </tr>
		   <tr>
		     <td  width="100" height="30" align="center" > HV Value (V) </td>

		   </tr>
		 </table>
	       </td>

	       <td>
		  <table border="1">
		   <tr>
		     <td rowspan="2"> </td>
		     
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT0"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT1"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT2"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT3"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT4"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT5"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT6"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT7"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT8"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="MASK_PMT9"/>        </td>
 	 	       
		   </tr>
		   
		   <tr>

		      <xsl:choose>
		       <xsl:when test="HV_PMT0_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT0"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT0"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT1_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT1"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT1"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT2_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT2"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT2"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="HV_PMT3_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT3"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT3"/>        </td>
		       </xsl:otherwise>
		    </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="HV_PMT4_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT4"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT4"/>        </td>
		       </xsl:otherwise>
		    </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="HV_PMT5_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT5"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT5"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT6_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT6"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT6"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT7_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT7"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT7"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT8_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT8"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT8"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT9_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="80" height="30" align="center"> <xsl:value-of select="HV_PMT9"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT9"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		   </tr>
		 </table>
	       </td>
		 
	       
	        <td>
		  <table border="1">
		   <tr>
		     <td rowspan="2"> </td>
	
		       <td width="100" height="30" align="center"><xsl:value-of select="MASK_PLANE_A"/>        </td>
		       <td width="100" height="30" align="center"><xsl:value-of select="MASK_PLANE_B"/>        </td>

		   </tr>

		   <tr>

		     <xsl:choose>
		       <xsl:when test="HV_PLANE_A_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="HV_PLANE_A"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="HV_PLANE_A"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="HV_PLANE_B_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="HV_PLANE_B"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="HV_PLANE_B"/>        </td>
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
