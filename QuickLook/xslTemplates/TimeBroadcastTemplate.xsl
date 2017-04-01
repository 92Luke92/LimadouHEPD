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
	     <td  width="100" height="30" align="center"> OBDH Timestamp (ms) </td>
	  
	       
	     
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> Absolute time (ms) </td>
		 </tr>
		 <tr>
		   <td width="150" height="30" align="center"> Time Start Run </td>
		   <td width="150" height="30" align="center"> Time Stop Run  </td>
		   
		 </tr>
	       </table>
	     </td>

	    

	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> Relative time (ms) </td>
		 </tr>
		 <tr>
		   <td width="150" height="30" align="center"> Time Start Run (ms) </td>
		   <td width="150" height="30" align="center"> Time Stop Run (ms) </td>
		    

		 </tr>
	       </table>
	     </td>
	   </tr>

	   
	   <xsl:for-each select="//BROADCAST">

	     <tr>
	       <td>
		 <table border="1">
		   <tr>
	              <td width="100" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	              <td width="100" height="30" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		   </tr>
		 </table>
	       </td>

	       <td width="150" height="30" align="center"><xsl:value-of select="OBDH_MS"/>        </td>
	       <td width="150" height="30" align="center"><xsl:value-of select="TIMESTAMP_OBDH"/>        </td>
	     
	         <td>
		 <table border="1">
		   <tr>
	              <td width="150" height="30" align="center"><xsl:value-of select="ABS_START_RUN"/>        </td>
	              <td width="150" height="30" align="center"><xsl:value-of select="ABS_STOP_RUN"/>         </td>
		     
		      
		   </tr>
		 </table>
		 </td>

		 		 

		 <td>
		 <table border="1">
		   <tr>
	              <td width="150" height="30" align="center"><xsl:value-of select="RELATIVE_START_RUN"/>        </td>
	              <td width="150" height="30" align="center"><xsl:value-of select="RELATIVE_STOP_RUN"/>         </td>
		     
		      
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
