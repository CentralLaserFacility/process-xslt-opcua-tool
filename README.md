This XSLT tool is Free and OpenSource provided under the terms of a BSD 3 licence.

Saxonica is the dependant library and the 'HomeEdition v11+' licence as of 2025 is available for use under Mozilla Public v2.
You will need to install a copy of these software for your platform prior to making this tool.

- Saxon Library source code: https://github.com/Saxonica/Saxon-HE/blob/main/12/Java/SaxonHE12-5J.zip
- The built Binaries/API = https://www.saxonica.com/saxon-c/documentation12/index.html

From the running directory, a typical command for generating EPICS Db or substitions files:

   `./SaxonTool_0/process_xml ../XML/1_myPlcOpcuaFile.xml ../2_XSLT_folder ../3_Output_folder`

PARAMS:

0. the binary executable
1. the XML file for a single PLC - e.g.  _SiOME_siemens_Room_228_big_accelerator.xml_
2. directory of developed XSLT files it will apply to the file specified in the parameter 1
3. per named XSLT sheet  e.g. _XSLT_translation_sheet_for_dual_gated_pneumatic_flippers.output_

NOTE:
You will need to create a batch/shell script to rename each of the outputs from point 3 so they have a .db or .substitution file extension that you might be creating for the IOC.
