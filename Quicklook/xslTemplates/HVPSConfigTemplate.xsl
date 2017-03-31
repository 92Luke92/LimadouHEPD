<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: RunHeaderTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Maurizio Nagni
     Descr.: Format the output of the RunHeaderToXml.c script to generate a human 
             readble verion of the ArrDump packet
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->

<!--Contains all the customer specific parameters-->
<!--<xsl:include href="Configure.xsl"/>-->
   
   <!-- Define which a key called "varDes" 
   	which will search inside the node "var" of the reference
   	ArrDumpInfo.xml (or whatever file instead of the is used)
	all values values of the node "idx" matching the valued passed when 
	the key "fileName" is called -->
   <!-- No xml file is defined here because the xsl:key is absolutely generic -->
<!--   <xsl:key name="arrDes" match="arr" use="idx"/>

   <xsl:template match="/" name="ArrDumpReference">
     <xsl:param name="code"/>
     <xsl:param name="arrval"/>
     <xsl:for-each select="document('./compilationInfo/PRH_ParamHandler_INFN_auto.arr.xml')">
       <xsl:for-each select="key('arrDes', $code)">
	 <td> <th align="left"><xsl:value-of select="name"/><xsl:text>(</xsl:text><xsl:value-of select="idx"/><xsl:text>)</xsl:text> </th> </td>
	 <td> <th align="left"><xsl:copy-of select="$arrval"/></th> </td>
	 <td> <th align="left"><xsl:value-of select="comment"/></th> </td>
       </xsl:for-each> 
     </xsl:for-each>
   </xsl:template>
-->
   <xsl:template match="/" name="ArrDumpTemplate">
     <html>
       <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	 <h2>HVPS Configuration </h2>
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
	       
	     <td width="100" height="30" align="center">   </td>
	     <td>
	       <table border="1">
		 <tr  bgcolor="#1acd32">
		   <td height="30" align="center" colspan="10">PMT HVPS ID</td>
		 </tr>
		 <tr>
		   <td width="80" height="30" align="center"> 0 </td>
		   <td width="80" height="30" align="center"> 1  </td>
		   <td width="80" height="30" align="center"> 2  </td>    
		   <td width="80" height="30" align="center"> 3  </td>   
		   <td width="80"  height="30" align="center"> 4  </td>    
		   <td width="80" height="30" align="center"> 5  </td> 
		   <td width="80" height="30" align="center"> 6  </td>  
		   <td width="80" height="30" align="center"> 7 </td>  
		   <td width="80" height="30" align="center"> 8 </td> 
		   <td width="80" height="30" align="center"> 9 </td>  
		 </tr>
	       </table>
	     </td>
	     <td>
	       <table border="1">
		 <tr bgcolor="#1acd32">
		   <td height="30" align="center" colspan="2"> Silicon Plane </td>
		 </tr>
		 <tr  bgcolor="#1acd32">
		   <td width="100" height="30" align="center">A</td>
		   <td width="100" height="30" align="center">B</td>
		 </tr>
	       </table>
	     </td>
	   </tr>

	   
	   <xsl:for-each select="//HVPSCONFIG">

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

		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT0"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT1"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT2"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT3"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT4"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT5"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT6"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT7"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT8"/>        </td>
		       <td width="80" height="30" align="center"><xsl:value-of select="HV_PMT9"/>        </td>

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
		    		       
		       <td width="100" height="30" align="center"><xsl:value-of select="HV_PLANE_A"/>        </td>
		       <td width="100" height="30" align="center"><xsl:value-of select="HV_PLANE_B"/>        </td>
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
