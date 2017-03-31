<?xml version="1.0" encoding="UTF-8"?>    <!-- Prologo XML -->
 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">   <!-- Dichiarazione del documento XSLT -->
<!--    <xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/TR/WD-xsl"> -->
    
<!--    
=====================================================================================
 Stylesheet: ScintConfigTemplate.xsl
    Version: 1.0 (2005-03-14)
     Author: Francesco Palma, Simona Bartocci
     Descr.: Format the output of the ScintConfigToXML.C 
=====================================================================================
-->
    
<!--Contains some utility to converts dec/hex/bin numbers-->
<!-- <xsl:include href="numberutils_lib.xsl"/> -->
  <xsl:template match="/" name="ArrDumpTemplate">
    <html>
      <body STYLE="font-family:Arial, helvetica, sans-serif; font-size:12pt">
	<h2>Scintillator Configuration </h2>
	
	<xsl:for-each select="//SCINTCONFIG">
	  
	  <table border="1">
	    <tr bgcolor="#1acd32">
	      <td>
		<table border="1">
		  <tr  bgcolor="#1acd32">
	            <td height="30" align="center" colspan="2"> <strong> Run ID </strong> </td>
		  </tr>
		  <tr>
	            <td width="150" height="30" align="center"><strong>Boot Number</strong></td>
	            <td width="150" height="30" align="center"><strong>Run Number</strong></td>
		  </tr>
		</table>
	      </td>
	    </tr>
	    
	    <tr>
	      <td>
		<table border="1">
		  <tr>
	            <td width="150" height="30" align="center"><xsl:value-of select="BOOT_NR"/>        </td>
	            <td width="150" height="30" align="center"><xsl:value-of select="RUN_NR"/>         </td>
		  </tr>
		</table>
	      </td>
	    </tr>
	  </table>

	  <table>
	    <tr bgcolor="#999999">
	      <td>
		<table border="1">
		  <tr  bgcolor="#999999">
		    <td height="30" align="center" colspan="2"><strong>Current Trigger Mask e  Veto </strong> </td>
		  </tr>
		  <tr  bgcolor="#999999">
		    <td width="150" height="30" align="center"> <strong>Veto </strong></td>
		    <td width="150" height="30" align="center"><strong> Trigger Mask</strong> </td> 
		  </tr>
		</table>
	      </td>
	      

	      <td>
		<table border="1">
		  <tr  bgcolor="#999999">
		    <td height="30" align="center" colspan="2"><strong> EASIROC Config: only thresholds </strong> </td>
		  </tr>
		  <tr  bgcolor="#999999">
		    <td width="150" height="30" align="center"><strong> EASIROC 1</strong> </td>
		    <td width="150" height="30" align="center"><strong> EASIROC 2</strong> </td> 
		  </tr>
		</table>
	      </td>
	    </tr>
	    
	    
	    
	    <tr>
	      <td>
		<table border="1">
		  <tr>
		    <td  width="150" height="30" align="center"><xsl:value-of select="VETO_CONFIG"/>        </td>
		    <td  width="150" height="30" align="center"><xsl:value-of select="CURRENT_TRIGGER_MASK"/>        </td>
                  </tr>
		</table>
	      </td>

	      <td>
		<table border="1">
		  <tr>
		    <td  width="150" height="30" align="center"><xsl:value-of select="EASIROC_1"/>        </td>
		    <td  width="150" height="30" align="center"><xsl:value-of select="EASIROC_2"/>        </td>
                  </tr>
		</table>
	      </td>	     
	    </tr>
	  </table>
	  
	  <table border="1">
	    <tr bgcolor="#999999">
	      <td  width="300" height="30" align="center"> <strong> Mask PMT </strong> </td> 
	    </tr>
	  </table>
	  

	  
	  <table border="1">
	    
	    <tr bgcolor="#999999">
	      <td  width="22" height="30" align="center"> T1E </td>
	      <td  width="22" height="30" align="center"> T2E </td>
	      <td  width="22" height="30" align="center"> T3E </td>
	      <td  width="22" height="30" align="center"> T4E </td>
	      <td  width="22" height="30" align="center"> T5E </td>
	      <td  width="22" height="30" align="center"> T6E </td>
	      <td  width="22" height="30" align="center"> P1SE </td>
	      <td  width="22" height="30" align="center"> P2SW </td>
	      <td  width="22" height="30" align="center"> P3SE </td>
	      <td  width="22" height="30" align="center"> P4SW </td>
	      <td  width="22" height="30" align="center"> P5SE </td>
	      <td  width="22" height="30" align="center"> P6SW </td>
	      <td  width="22" height="30" align="center"> P7SE </td>
	      <td  width="22" height="30" align="center"> P8SW </td>
	      <td  width="22" height="30" align="center"> P9SE </td>
	      <td  width="22" height="30" align="center"> P10SW </td>
	    </tr>
	    <tr>

	      <xsl:if test="MASK_PMT_0 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_0"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_0 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_0"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_1 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_1"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_1 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_1"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_2 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_2"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_2 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_2"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_3 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_3"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_3 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_3"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_4 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_4"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_4 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_4"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_5 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_5"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_5 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_5"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_6 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_6"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_6 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_6"/> </td>
	      </xsl:if>
	      
	      <xsl:if test="MASK_PMT_7 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_7"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_7 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_7"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_8 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_8"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_8 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_8"/> </td>
	      </xsl:if>
	      
	       <xsl:if test="MASK_PMT_9 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_9"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_9 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_9"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK_PMT_10 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_10"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_10 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_10"/> </td>
	      </xsl:if>
	      
	       <xsl:if test="MASK_PMT_11 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_11"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_11 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_11"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_12 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_12"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_12 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_12"/> </td>
	      </xsl:if>

	     <xsl:if test="MASK_PMT_13 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_13"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_13 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_13"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK_PMT_14 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_14"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_14 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_14"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_15 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_15"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_15 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_15"/> </td>
	      </xsl:if>

	    </tr>

	    
	    <tr bgcolor="#999999">
	      <td  width="22" height="30" align="center"> P11SE </td>
	      <td  width="22" height="30" align="center"> P12SW </td>
	      <td  width="22" height="30" align="center"> P13SE </td>
	      <td  width="22" height="30" align="center"> P14SW </td>
	      <td  width="22" height="30" align="center"> P15SE </td>
	      <td  width="22" height="30" align="center"> P16SW </td>	
	      <td  width="22" height="30" align="center"> VNu </td>
	      <td  width="22" height="30" align="center"> VEu </td>
	      <td  width="22" height="30" align="center"> VSu </td>
	      <td  width="22" height="30" align="center"> VWu </td>
	      <td  width="22" height="30" align="center"> VBNE </td>
	      <td  width="22" height="30" align="center"> L9SW </td>
	      <td  width="22" height="30" align="center"> L7NW </td>
	      <td  width="22" height="30" align="center"> L1NE </td>
	      <td  width="22" height="30" align="center"> L8W </td>
	      <td  width="22" height="30" align="center"> L5C </td>
	    </tr>	  
	    <tr>

	      <xsl:if test="MASK_PMT_16 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_16"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_16 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_16"/> </td>
	      </xsl:if>

	        <xsl:if test="MASK_PMT_17 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_17"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_17 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_17"/> </td>
	      </xsl:if>

	        <xsl:if test="MASK_PMT_18 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_18"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_18 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_18"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_19 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_19"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_19 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_19"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK_PMT_20 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_20"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_20 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_20"/> </td>
	      </xsl:if>

	         <xsl:if test="MASK_PMT_21 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_21"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_21 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_21"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_22 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_22"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_22 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_22"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK_PMT_23 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_23"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_23 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_23"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_24 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_24"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_24 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_24"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_25 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_25"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_25 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_25"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_26 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_26"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_26 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_26"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_27 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_27"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_27 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_27"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_28 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_28"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_28 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_28"/> </td>
	      </xsl:if>
	      
	      <xsl:if test="MASK_PMT_29 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_29"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_29 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_29"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_30 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_30"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_30 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_30"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK_PMT_31 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_31"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT_31 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK_PMT_31"/> </td>
	      </xsl:if>

	    </tr>
	    <tr bgcolor="#999999">
	      <td  width="22" height="30" align="center"> T1W </td>
	      <td  width="22" height="30" align="center"> T2W </td>
	      <td  width="22" height="30" align="center"> T3W </td>
	      <td  width="22" height="30" align="center"> T4W </td>
	      <td  width="22" height="30" align="center"> T5W </td>
	      <td  width="22" height="30" align="center"> T6W </td>
	      <td  width="22" height="30" align="center"> P1NW </td>
	      <td  width="22" height="30" align="center"> P2NE </td>
	      <td  width="22" height="30" align="center"> P3NW </td>
	      <td  width="22" height="30" align="center"> P4NE </td>
	      <td  width="22" height="30" align="center"> P5NW </td>
	      <td  width="22" height="30" align="center"> P6NE </td>
	      <td  width="22" height="30" align="center"> P7NW </td>
	      <td  width="22" height="30" align="center"> P8NE </td>
	      <td  width="22" height="30" align="center"> P9NW </td>
	      <td  width="22" height="30" align="center"> P10NE </td>
	    </tr>

	    <tr>

	      <xsl:if test="MASK2_PMT_0 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_0"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_0 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_0"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_1 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_1"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_1 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_1"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_2 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_2"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_2 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_2"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_3 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_3"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_3 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_3"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_4 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_4"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_4 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_4"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_5 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_5"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_5 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_5"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_6 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_6"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_6 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_6"/> </td>
	      </xsl:if>
	      
	      <xsl:if test="MASK2_PMT_7 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_7"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_7 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_7"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_8 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_8"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_8 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_8"/> </td>
	      </xsl:if>
	      
	       <xsl:if test="MASK2_PMT_9 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_9"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_9 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_9"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK2_PMT_10 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_10"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_10 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_10"/> </td>
	      </xsl:if>
	      
	       <xsl:if test="MASK2_PMT_11 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_11"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_11 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_11"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_12 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_12"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_12 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_12"/> </td>
	      </xsl:if>

	     <xsl:if test="MASK2_PMT_13 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_13"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_13 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_13"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK2_PMT_14 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_14"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_14 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_14"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_15 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_15"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_15 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_15"/> </td>
	      </xsl:if>

	    </tr>


	    
	    <tr bgcolor="#999999">
	      <td  width="22" height="30" align="center"> P11NW </td>
	      <td  width="22" height="30" align="center"> P12NE </td>
	      <td  width="22" height="30" align="center"> P13NW </td>
	      <td  width="22" height="30" align="center"> P14NE </td>
	      <td  width="22" height="30" align="center"> P15NW </td>
	      <td  width="22" height="30" align="center"> P16NE </td>
	      <td  width="22" height="30" align="center"> VNd </td>
	      <td  width="22" height="30" align="center"> VEd </td>
	      <td  width="22" height="30" align="center"> VSd </td>
	      <td  width="22" height="30" align="center"> VWd </td>
	      <td  width="22" height="30" align="center"> VBSW </td>
	      <td  width="22" height="30" align="center"> L3SE </td>
	      <td  width="22" height="30" align="center"> L2E </td>
	      <td  width="22" height="30" align="center"> L6S </td>
	      <td  width="22" height="30" align="center"> L4N </td>
	    </tr>
	    
	    <tr>



	      <xsl:if test="MASK2_PMT_16 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_16"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_16 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_16"/> </td>
	      </xsl:if>

	        <xsl:if test="MASK2_PMT_17 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_17"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_17 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_17"/> </td>
	      </xsl:if>

	        <xsl:if test="MASK2_PMT_18 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_18"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_18 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_18"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_19 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_19"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_19 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_19"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK2_PMT_20 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_20"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_20 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_20"/> </td>
	      </xsl:if>

	         <xsl:if test="MASK2_PMT_21 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_21"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_21 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_21"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_22 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_22"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_22 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_22"/> </td>
	      </xsl:if>

	       <xsl:if test="MASK2_PMT_23 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_23"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_23 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_23"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_24 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_24"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_24 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_24"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_25 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_25"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_25 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_25"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_26 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_26"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_26 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_26"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_27 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_27"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK_PMT2_27 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_27"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_28 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_28"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_28 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_28"/> </td>
	      </xsl:if>
	      
	      <xsl:if test="MASK2_PMT_29 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_29"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_29 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_29"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_30 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_30"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_30 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_30"/> </td>
	      </xsl:if>

	      <xsl:if test="MASK2_PMT_31 ='0'">
	        <td  bgcolor="#ff0000" width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_31"/> </td>
	      </xsl:if>
	      <xsl:if test="MASK2_PMT_31 ='1'">
	        <td width="22" height="30" align="center"> <xsl:value-of select="MASK2_PMT_31"/> </td>
	      </xsl:if>

	    </tr>
	  </table>
	  
	  <table border="1">
	    <tr bgcolor="#999999">
	      <td  width="300" height="30" align="center"> <strong> Generic Trigger Mask  </strong></td> 
	    </tr>
	  </table>
	  
	  <table border="1">
	    <tr bgcolor="#999999">
	      <td  width="22" height="30" align="center"> T </td>
	      <td  width="22" height="30" align="center"> P1 </td>
	      <td  width="22" height="30" align="center"> P2 </td>
	      <td  width="22" height="30" align="center"> P3 </td>
	      <td  width="22" height="30" align="center"> P4 </td>
	      <td  width="22" height="30" align="center"> P5 </td>
	      <td  width="22" height="30" align="center"> P6 </td>
	      <td  width="22" height="30" align="center"> P7 </td>
	      <td  width="22" height="30" align="center"> P8 </td>
	      <td  width="22" height="30" align="center"> P9 </td>
	      <td  width="22" height="30" align="center"> P10 </td>
	      <td  width="22" height="30" align="center"> P11 </td>
	      <td  width="22" height="30" align="center"> P12 </td>
	      <td  width="22" height="30" align="center"> P13 </td>
	      <td  width="22" height="30" align="center"> P14 </td>
	      <td  width="22" height="30" align="center"> P15 </td>
	      <td  width="22" height="30" align="center"> P16 </td>
	      <td  width="22" height="30" align="center"> L </td>
	    </tr>
	    <tr>
	      <td  width="22" height="30" align="center"><xsl:value-of select="GEN_TRIG_MASK_0"/>  </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_1"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_2"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_3"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_4"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_5"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_6"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_7"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_8"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_9"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_10"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_11"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_12"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_13"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_14"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_15"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_16"/> </td>
	      <td  width="22" height="30" align="center"> <xsl:value-of select="GEN_TRIG_MASK_17"/> </td>
	    </tr>
	  </table>
	  
	</xsl:for-each>   

      </body>
    </html>
  </xsl:template>
  
</xsl:stylesheet>







