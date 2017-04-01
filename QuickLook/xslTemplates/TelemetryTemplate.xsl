<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: TelemetryTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the TelemetryToXML.C
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->
   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>Telemetry</h2>
	 <table border="1">
	   <tr bgcolor="#1acd32">
	     <td>
	     <table border="1">
	       <tr bgcolor="#1acd32">
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
		   <td height="30" align="center" colspan="2"> DAQ Board </td>
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
		   <td height="30" align="center" colspan="2"> PMT/Trigger Board </td>
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
		   <td height="30" align="center" colspan="2"> TM/TC Power Board </td>
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
		   <td height="30" align="center" colspan="2"> HVPS Control Board </td>
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
		   <td height="30" align="center" colspan="2"> CPU Board Boot Status </td>
		 </tr>
		 <tr>
		   <td width="100" height="30" align="center"> Start  </td>
		   <td width="100" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>    
	   </tr>
	   
	   <tr bgcolor="#339966">
	     <td width="200" height="30" align="center">Default Status Values</td>
	      <td>
		 <table border="1">
		   <tr>
		     <td width="100" height="30" align="center"> 0X50A1  </td>
		     <td width="100" height="30" align="center"> 0X80A3  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>	     
		     <td width="100" height="30" align="center"> 0XD05  </td>
		     <td width="100" height="30" align="center"> 0XF1F  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>   
		     <td width="100" height="30" align="center"> 0XFFD  </td>
		     <td width="100" height="30" align="center"> 0XFFD  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>        
		     <td width="100" height="30" align="center"> 0  </td>
		     <td width="100" height="30" align="center"> 0  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>      
		     <td width="100" height="30" align="center"> 0X704  </td>
		     <td width="100" height="30" align="center"> 0X704  </td>
		   </tr>
		 </table>
	      </td>
	   </tr>
	  
	   
	   <xsl:for-each select="//TELEMETRY">

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
		     		     
		     <xsl:choose>
		       <xsl:when test="DAQ_START_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="DAQ_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="DAQ_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="DAQ_STOP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="DAQ_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="DAQ_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
 
		   </tr>
		 </table>
		 </td>
		 
		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="PMT_START_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="PMT_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="PMT_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="PMT_STOP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="PMT_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="PMT_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		           
		   </tr>
		 </table>
		 </td>

		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="TM_TC_START_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="TM_TC_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="TM_TC_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="TM_TC_STOP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="TM_TC_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="TM_TC_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		   </tr>
		 </table>
		 </td>

		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="HVPS_START_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="HVPS_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="HVPS_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HVPS_STOP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="HVPS_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="HVPS_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		   </tr>
		 </table>
		 </td>

		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="CPU_START_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="CPU_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CPU_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="CPU_STOP_error &gt;'0'">
			 <td  bgcolor="#ff0000" width="100" height="30" align="center"> <xsl:value-of select="CPU_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="CPU_STOP"/>        </td>
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
