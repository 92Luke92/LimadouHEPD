<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: TimeBroadcastTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the BroadcastToXML.C 
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->

   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>Time Broadcast</h2>
	 <table border="1">
	   <tr bgcolor="#1acd32">

	     <td>
	       <tr  bgcolor="#1acd32">
	         <td></td>
	       </tr>
	     </td>

	     <td>
	       <table border="1">
		 <tr>
	     <td height="60" align="center" colspan="2"> Run ID </td>
	       </tr>
	       <tr>
	         <td width="60" height="30" align="center">Boot nr.</td>
	         <td width="60" height="30" align="center">Run nr</td>
	       </tr>
	     </table>
	     </td>

	     <td  width="90" height="30" align="center"> OBDH Time Broadcast (yyyy-mm-dd hh:mm:ss) </td>
	     <td  width="70" height="30" align="center"> OBDH Time Broadcast (ms) </td>
	     <td  width="70" height="30" align="center"> CPU-timestamp of OBDH broadcast (ms) </td>
	     <td  width="90" height="30" align="center"> AOCC Time Broadcast (yyyy-mm-dd hh:mm:ss) </td>
	     <td  width="70" height="30" align="center"> AOCC Time Broadcast (μs) </td>
	     <td  width="70" height="30" align="center"> CPU-timestamp of AOCC broadcast (ms) </td>
	  
	      
	     
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="3"> Absolute time  </td>
		 </tr>		 
		 <tr>
		    <td width="100" height="30" align="center"> Time  </td> 
		   <td width="200" height="30" align="center"> (yyyy-mm-dd hh:mm:ss) </td>
		   <td  width="100" height="30" align="center"> (ms) </td>
		 </tr>
			 
	       </table>
	     </td>
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> CPU Time  </td>
		 </tr>		 
		 <tr>
		    <td width="100" height="30" align="center"> Time   </td> 
 		   <td  width="100" height="30" align="center"> (ms) </td>
		 </tr>
			 
	       </table>
	     </td>
	    
		  

	   </tr>

	   
	   <xsl:for-each select="//BROADCAST">

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
	
		   <tr>

		     <xsl:choose>
		       <xsl:when test="BROADCAST_OBDH_SEC_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="90" height="30" align="center"> <xsl:value-of select="OBDH_S"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="90" height="30" align="center"><xsl:value-of select="OBDH_S"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		   </tr>
	
	       </td>


	       <td>
	
		   <tr>

		     <xsl:choose>
		       <xsl:when test="BROADCAST_OBDH_MS_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="90" height="30" align="center"> <xsl:value-of select="OBDH_MS"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="90" height="30" align="center"><xsl:value-of select="OBDH_MS"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		   </tr>
	
	       </td>

	       <td>
		
		   <tr>

		     <xsl:choose>
		       <xsl:when test="TIMESTAMP_OBDH_Y &gt;'0'">
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="TIMESTAMP_OBDH"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="100" height="30" align="center"><xsl:value-of select="TIMESTAMP_OBDH"/>        </td>
		       </xsl:otherwise>
		     </xsl:choose>
		      
		   </tr>
	
	       </td>

	       <td width="90" height="30" align="center"><xsl:value-of select="AOCC_S"/>        </td>
	       <td width="70" height="30" align="center"><xsl:value-of select="AOCC_US"/>        </td>
	       <td width="70" height="30" align="center"><xsl:value-of select="TIMESTAMP_AOCC"/>        </td>
	       
	      


		<td>
		  <table border="1">
		   <tr>
		     <xsl:choose>
		       <xsl:when test="ABS_TIME_Y &gt;'0'">
			 <td width="70" height="30" align="center"><xsl:value-of select="TIME_RUN"/>        </td>
			 <td  bgcolor="#ffff00" width="200" height="30" align="center"> <xsl:value-of select="ABS_TIME"/> </td>
			 <td  bgcolor="#ffff00" width="100" height="30" align="center"> <xsl:value-of select="ABS_TIME_ms"/> </td>
		       </xsl:when>
		       <xsl:otherwise>
			 <td width="70" height="30" align="center"><xsl:value-of select="TIME_RUN"/>        </td>
			 <td width="200" height="30" align="center"><xsl:value-of select="ABS_TIME"/>        </td>
			 <td width="100" height="30" align="center"><xsl:value-of select="ABS_TIME_ms"/>        </td>
		       </xsl:otherwise>
		      </xsl:choose>
	              	 	       
		   </tr>
		   	   
		 </table>
		</td>

		<td>
		  <table border="1">
		   <tr>

		     <td width="70" height="30" align="center"><xsl:value-of select="TIME_RUN"/>        </td>
		     <td width="100" height="30" align="center"><xsl:value-of select="CPU_TIME"/>         </td>
			             	 	       
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
