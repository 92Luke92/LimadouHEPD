<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: RunInfoTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the housekeeping.cc - Run information
=====================================================================================
-->
    

<!-- <xsl:include href="numberutils_lib.xsl"/> -->

   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>DUMP Configurations </h2>


	   
	   <xsl:for-each select="//DUMP_CONFIG"> 

	     <xsl:if test="INDEX_2 ='1'">
	       
	       <table border="1">
		 <tr bgcolor="#1acd32">
		   <td>
		     <table border="1">
		       <tr  bgcolor="#1acd32">
			 <td height="30" align="center" colspan="2"> <strong> Run ID </strong> </td>
		       </tr>
		       <tr>
			 <td width="80" height="30" align="center"><strong>Boot nr.</strong></td>
			 <td width="80" height="30" align="center"><strong>Run nr.</strong></td>
		       </tr>
		     </table>
		   </td>
		 </tr>

		 <tr>
		   <td>
		     <table border="1">
		       <tr>
			 <td width="80" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
			 <td width="80" height="30" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		       </tr>
		     </table>
		   </td>
		 </tr>
	       </table>



	       <table border="1">
		 <tr bgcolor="#999999">
		   <td>
		     <table border="1">
		       <tr  bgcolor="#999999">
			 <td height="30" align="center" colspan="6"> <strong> Orbit Zones Configuration </strong> </td>
		       </tr>
		       <tr>
			 
			 <td>
		   	   <tr  bgcolor="#999999">
			     <td width="120" height="40" align="center">Orbit Zone</td>
			   </tr>
			 </td>

			 <td>
			   <table border="1">
			     <tr bgcolor="#999999">
			       <td width="60" height="40" align="center" colspan="3">SD Ladder Mask </td>
			     </tr>
			     <tr>
			       <td width="60" height="40" align="center"> Top </td>
			       <td width="60" height="40" align="center"> Centr. </td>
			       <td width="60" height="40" align="center"> Bott. </td>
			     </tr>
			   </table>
			 </td>

			 <td>
			   <table border="1">
			     <tr bgcolor="#999999">
			       <td width="120" height="40" align="center" colspan="2">Trigger Mask and Veto </td>
			     </tr>
			     <tr>
			       <td width="80" height="40" align="center"> Veto </td>
			       <td width="80" height="40" align="center"> Trigger Mask </td>
			     </tr>
			   </table>
			 </td>

		   <td width="100" height="30" align="center"> Run duration (s) </td>
		   <td width="150" height="30" align="center"> Latitude Range </td> 
		   <td width="150" height="30" align="center"> Longitude Range </td>
			 

			 
		       </tr>
		     </table>
		   </td>
		 </tr>


	       </table>

	     
	
	     </xsl:if>
	     
	     <tr>
	       <table border="1">
		 <td width="130" height="30" align="center"><xsl:value-of select="ORBIT_CONF"/>        </td>
		 <td width="63" height="30" align="center"><xsl:value-of select="LADDER_MASK_TOP"/>         </td>
		 <td width="63" height="30" align="center"><xsl:value-of select="LADDER_MASK_CENTR"/>         </td>
		 <td width="63" height="30" align="center"><xsl:value-of select="LADDER_MASK_BOTT"/>         </td>
		 <td width="85" height="30" align="center"><xsl:value-of select="TRIGGER_MASK_VETO"/>         </td>
		 <td width="85" height="30" align="center"><xsl:value-of select="TRIGGER_MASK"/>         </td>
		 <td width="105" height="30" align="center"><xsl:value-of select="RUN_DURATION"/>         </td>
		 <td width="150" height="30" align="center"><xsl:value-of select="LATITUDE_RANGE"/>         </td>
		 <td width="150" height="30" align="center"><xsl:value-of select="LONGITUDE_RANGE"/>         </td>
	       </table>
	       
	     </tr>

	     <xsl:if test="INDEX ='1'">

	       <table border="1">
		 <tr bgcolor="#999999">
		   <td>
		     <table border="1">
		       <tr  bgcolor="#999999">
			 <td height="30" align="center" colspan="4"> <strong> Other Configurations </strong> </td>
		       </tr>
		       <tr>
			 <td width="180" height="30" align="center">Force Defaut zone </td>
			 <td width="180" height="30" align="center">WO Config. ID </td>
			 <td width="180" height="30" align="center">Calibration period </td>
			 <td width="180" height="30" align="center">Safe Mode Flag </td>
		       </tr>
		     </table>
		   </td>
		 </tr>
		 
		 <tr>
		   <td>
		     <table border="1">
		       <tr>
			 <td width="180" height="30" align="center"><xsl:value-of select="ORBITAL_SETTINGS"/>         </td>
			 <td width="180" height="30" align="center"><xsl:value-of select="CONFIG_ID"/>         </td>
			 <td width="180" height="30" align="center"><xsl:value-of select="CALIB_PERIOD"/>         </td>
			 <td width="180" height="30" align="center"><xsl:value-of select="SAFE_MODE"/>         </td>
		       </tr>
		     </table>
		   </td>
		 </tr>
	       </table>
      
	     </xsl:if>

	   </xsl:for-each>


	 
      </body>
     </html>
   </xsl:template>
   
 </xsl:stylesheet>
