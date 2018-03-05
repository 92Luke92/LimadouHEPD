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
		   <td width="165" height="60" align="center"> 0 - (T1e,T3e,T5e,P15se, VEu,VSu,VBne) </td>
		   <td width="170" height="60" align="center"> 1 - (VNu,VWu,T1w,T3w, T5w,P15nw,VBsw)  </td>
		   <td width="120" height="60" align="center"> 2 - (T2e,T4e,T6e, P16ne,VEd,VSd)  </td>    
		   <td width="120" height="60" align="center"> 3 - (P16sw,T2w,T4w, T6w,VNd,VWd)  </td>   
		   <td width="180"  height="60" align="center"> 4 -  (P3se,P4sw,P11se,P1nw, P2ne,P9nw,P10ne)  </td>    
		   <td width="195" height="60" align="center"> 5 -  (P7se,P8sw,P13se,P14sw, P5nw,P6ne,P12ne)  </td> 
		   <td width="195" height="60" align="center"> 6 -  (P1se,P2sw,P9se,P10sw, P9nw,P4ne,P11nw)  </td>  
		   <td width="195" height="60" align="center"> 7 -  (P1se,P2sw,P9se,P10sw, P8nw,P4ne,P11nw) </td>  
		   <td width="130" height="60" align="center"> 8 - (L8w,L2e,L6s,L4n) </td> 
		   <td width="130" height="60" align="center"> 9 - (L9sw,L7nw,L1ne, L5c,L3se) </td>  
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
	              <td width="60" height="90" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="60" height="90" align="center"><xsl:value-of select="RUN_NR"/>         </td>
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
		     <td  width="100" height="50" align="center" > HV Value (V) </td>

		   </tr>
		 </table>
	       </td>

	       <td>
		  <table border="1">
		   <tr>
		     <td rowspan="2"> </td>
		     

		     <xsl:choose>
		       <xsl:when test="MASK_PMT0_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="160" height="30" align="center"> <xsl:value-of select="MASK_PMT0"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="160" height="30" align="center"><xsl:value-of select="MASK_PMT0"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="MASK_PMT1_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="158" height="30" align="center"> <xsl:value-of select="MASK_PMT1"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="158" height="30" align="center"><xsl:value-of select="MASK_PMT1"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="MASK_PMT2_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="135" height="30" align="center"> <xsl:value-of select="MASK_PMT2"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="135" height="30" align="center"><xsl:value-of select="MASK_PMT2"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="MASK_PMT3_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="130" height="30" align="center"> <xsl:value-of select="MASK_PMT3"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="130" height="30" align="center"><xsl:value-of select="MASK_PMT3"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="MASK_PMT4_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="30" align="center"> <xsl:value-of select="MASK_PMT4"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="30" align="center"><xsl:value-of select="MASK_PMT4"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>

		       <xsl:choose>
		       <xsl:when test="MASK_PMT5_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="205" height="30" align="center"> <xsl:value-of select="MASK_PMT5"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="205" height="30" align="center"><xsl:value-of select="MASK_PMT5"/>        </td>
		       </xsl:otherwise>
		       </xsl:choose>

		        <xsl:choose>
		       <xsl:when test="MASK_PMT6_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="30" align="center"> <xsl:value-of select="MASK_PMT6"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="30" align="center"><xsl:value-of select="MASK_PMT6"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="MASK_PMT7_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="30" align="center"> <xsl:value-of select="MASK_PMT7"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="30" align="center"><xsl:value-of select="MASK_PMT7"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>

		       <xsl:choose>
		       <xsl:when test="MASK_PMT8_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="140" height="30" align="center"> <xsl:value-of select="MASK_PMT8"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="140" height="30" align="center"><xsl:value-of select="MASK_PMT8"/>        </td>
		       </xsl:otherwise>
		       </xsl:choose>

		        <xsl:choose>
		       <xsl:when test="MASK_PMT9_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="140" height="30" align="center"> <xsl:value-of select="MASK_PMT9"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="140" height="30" align="center"><xsl:value-of select="MASK_PMT9"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
 	       
		   </tr>
		   
		   <tr>

		      <xsl:choose>
		       <xsl:when test="HV_PMT0_error &gt;'0' or MASK_PMT0_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="160" height="50" align="center"> <xsl:value-of select="HV_PMT0"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="160" height="50" align="center"><xsl:value-of select="HV_PMT0"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT1_error &gt;'0' or MASK_PMT1_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="158" height="50" align="center"> <xsl:value-of select="HV_PMT1"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="158" height="50" align="center"><xsl:value-of select="HV_PMT1"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT2_error &gt;'0' or MASK_PMT2_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="135" height="50" align="center"> <xsl:value-of select="HV_PMT2"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="135" height="50" align="center"><xsl:value-of select="HV_PMT2"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="HV_PMT3_error &gt;'0' or MASK_PMT3_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="130" height="50" align="center"> <xsl:value-of select="HV_PMT3"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="130" height="50" align="center"><xsl:value-of select="HV_PMT3"/>        </td>
		       </xsl:otherwise>
		    </xsl:choose>

		    <xsl:choose>
		       <xsl:when test="HV_PMT4_error &gt;'0' or MASK_PMT4_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="50" align="center"> <xsl:value-of select="HV_PMT4"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="50" align="center"><xsl:value-of select="HV_PMT4"/>        </td>
		       </xsl:otherwise>
		    </xsl:choose>

		    <xsl:choose>
		      <xsl:when test="HV_PMT5_error &gt;'0' or MASK_PMT5_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="205" height="50" align="center"> <xsl:value-of select="HV_PMT5"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="205" height="50" align="center"><xsl:value-of select="HV_PMT5"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT6_error &gt;'0' or MASK_PMT6_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="50" align="center"> <xsl:value-of select="HV_PMT6"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="50" align="center"><xsl:value-of select="HV_PMT6"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT7_error &gt;'0' or MASK_PMT7_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="195" height="50" align="center"> <xsl:value-of select="HV_PMT7"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="195" height="50" align="center"><xsl:value-of select="HV_PMT7"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT8_error &gt;'0' or MASK_PMT8_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="140" height="50" align="center"> <xsl:value-of select="HV_PMT8"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="140" height="50" align="center"><xsl:value-of select="HV_PMT8"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HV_PMT9_error &gt;'0' or MASK_PMT9_OFF &gt;'0'">
			 <td  bgcolor="#ff0000" width="140" height="50" align="center"> <xsl:value-of select="HV_PMT9"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="140" height="50" align="center"><xsl:value-of select="HV_PMT9"/>        </td>
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
			 <td  bgcolor="#ff0000" width="100" height="50" align="center"> <xsl:value-of select="HV_PLANE_A"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="50" align="center"><xsl:value-of select="HV_PLANE_A"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="HV_PLANE_B_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="50" align="center"> <xsl:value-of select="HV_PLANE_B"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="50" align="center"><xsl:value-of select="HV_PLANE_B"/>        </td>
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
