function doGet(e) 
{
  /* Layout of URL:
        "/macros/s/--GOOGLE_DOCS_URL--/exec?ID=SHEET_NAME&TMP=12345

        ID is the name of the sheet to put the data 
        TMP is the value of the sensor
  */ 

  // URL TO SPREAD SHEET
  var ss = SpreadsheetApp.openByUrl('https://docs.google.com/spreadsheets/d/SOMETHING_SOMETHING/edit#gid=0');
  var sheet = ss.getSheetByName(e.parameter.ID);  

  var CurrentDate = new Date();
  var Time_ = Utilities.formatDate(CurrentDate, ss.getSpreadsheetTimeZone(), "MM/dd/YYYY HH:mm:ss");
  
  // DSP sensor can sometimes read -127, this is to avoid adding it to spreadsheet
  if (e.parameter.TMP == -127)
  {
    return ContentService.createTextOutput('Error with sensor!');
  }
  
  var rowData = [Time_, e.parameter.TMP];
  /* This value is used to limit the number of rows of data */
  /* 10080 data points is about 1 week worth of data at 2 minute intervals */
  var lastRow = 10080; /* This is the max number of data to collect */
  
  /* Move all current data down one row without running off the max lastRow value */
  var target = sheet.getRange(2, 1, lastRow, 2);
  sheet.getRange(1, 1, lastRow - 1, 2).copyTo(target);
  
  /* Add Data to top of sheet */
  sheet.getRange(1, 1, 1, rowData.length).setValues([rowData]);
  
  /* This will fill most recent data at top of sheet */
  //var newRange = sheet.insertRowBefore(2).getRange(1, 1, 1, rowData.length);

  return ContentService.createTextOutput('Success!');
}