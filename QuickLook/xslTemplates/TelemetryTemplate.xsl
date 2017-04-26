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
	 <h2>Run Telemetry</h2>
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
		   <td width="120" height="30" align="center"> Start  </td>
		   <td width="400" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> PMT/Trigger Board </td>
		 </tr>
		 <tr>
		   <td width="210" height="30" align="center"> Start  </td>
		   <td width="190" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> TM/TC Power Board </td>
		 </tr>
		 <tr>
		   <td width="150" height="30" align="center"> Start  </td>
		   <td width="150" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	     </td>

	       <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> HVPS Control Board </td>
		 </tr>
		 <tr>
		   <td width="150" height="30" align="center"> Start  </td>
		   <td width="150" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	       </td>

	       <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> CPU Board Boot Status </td>
		 </tr>
		 <tr>
		   <td width="150" height="30" align="center"> Start  </td>
		   <td width="150" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	       </td>

	       <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> CPU Board Status FSM </td>
		 </tr>
		 <tr>
		   <td width="370" height="30" align="center"> Start  </td>
		   <td width="230" height="30" align="center"> Stop  </td>
		 </tr>
	       </table>
	       </td>
	       
	   </tr>
	   
	   <tr bgcolor="#339966">
	     <td width="200" height="30" align="center">Default Status Values (Approximate Values)</td>
	      <td>
		 <table border="1">
		   <tr>
		     <td width="100" height="60" align="center"> 0X50A1  </td>
		     <td width="400" height="60" align="center"> 0X80A3/70A2/60A2 </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>	     
		     <td width="210" height="60" align="center"> 0X0D00/0D05 </td>
		     <td width="190" height="60" align="center"> 0Xxx1F/xx2F </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>   
		     <td width="150" height="60" align="center"> 0XFFD/75C  </td>
		     <td width="150" height="60" align="center"> 0XFFD/75C  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>        
		     <td width="150" height="60" align="center"> 0x000/004  </td>
		     <td width="150" height="60" align="center"> 0x000/004  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>      
		     <td width="150" height="60" align="center"> 0x704/705  </td>
		     <td width="150" height="60" align="center"> 0x704/705  </td>
		   </tr>
		 </table>
	      </td>

	      <td>
		 <table border="1">
		   <tr>      
		     <td width="370" height="60" align="center"> 0X300/100/200 </td>
		     <td width="230" height="60" align="center"> 0X001/301/302/002  </td>
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
			 <td  bgcolor="#ffff00" width="60" height="30" align="center"> <xsl:value-of select="DAQ_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="60" height="30" align="center"><xsl:value-of select="DAQ_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		      <xsl:choose>
		       <xsl:when test="DAQ_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="400" height="30" align="center"> <xsl:value-of select="DAQ_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="400" height="30" align="center"><xsl:value-of select="DAQ_STOP"/>        </td>
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
			 <td  bgcolor="#ffff00" width="210" height="30" align="center"> <xsl:value-of select="PMT_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="210" height="30" align="center"><xsl:value-of select="PMT_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="PMT_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="190" height="30" align="center"> <xsl:value-of select="PMT_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="190" height="30" align="center"><xsl:value-of select="PMT_STOP"/>        </td>
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
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="TM_TC_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="TM_TC_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="TM_TC_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="TM_TC_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="TM_TC_STOP"/>        </td>
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
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="HVPS_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="HVPS_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="HVPS_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="HVPS_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="HVPS_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		   </tr>
		 </table>
		 </td>

		 <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="CPUboot_START_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="CPUboot_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="CPUboot_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="CPUboot_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="150" height="30" align="center"> <xsl:value-of select="CPUboot_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="150" height="30" align="center"><xsl:value-of select="CPUboot_STOP"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		   </tr>
		 </table>
		 </td>

		  <td>
		 <table border="1">
		   <tr>

		     <xsl:choose>
		       <xsl:when test="CPU_FSM_START_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="370" height="30" align="center"> <xsl:value-of select="CPU_FSM_START"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="370" height="30" align="center"><xsl:value-of select="CPU_FSM_START"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>

		     <xsl:choose>
		       <xsl:when test="CPU_FSM_STOP_error &gt;'0'">
			 <td  bgcolor="#ffff00" width="230" height="30" align="center"> <xsl:value-of select="CPU_FSM_STOP"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="230" height="30" align="center"><xsl:value-of select="CPU_FSM_STOP"/>        </td>
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
