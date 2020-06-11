#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sstream>

#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TCutG.h"
#include "TRandom3.h"
#include "TObject.h"
#include "TROOT.h"

using namespace std;

typedef std::vector<int> i_vector;

TH2D* backgroundSubtraction(TH2D* histIn, TH2D* bcg, double weight, double sigmaX, double sigmaY); // older method
TH2D* backgroundSubtractionMask(TH2D* histIn, TH2D* bcg, double weight, double sigmaX, double sigmaY); // newer method

int checkRanges(TH2D* histo, TH2D* bcg);

/** Improved matrix subtraction. Background bins are weighted with the 
 * 2D Gaussian mask. **/
TH2D* backgroundSubtractionMask(TH2D* histIn, TH2D* bcg, double weight, double sigmaX, double sigmaY)
{
  int xBins, yBins;
  double xMin, xMax, yMin, yMax;

  double xTemp = 0.0;
  double yTemp = 0.0;
  double binWidthX;
  double binWidthY;
  
  TRandom3 rnd(0);
  
  if( checkRanges( histIn, bcg) == 0) return 0; // checks, if the matrices have the same X and Y axes. 
  
  xBins =  histIn->GetNbinsX();
  yBins =  histIn->GetNbinsY();
  xMin =  histIn->GetXaxis()->GetXmin();
  yMin =  histIn->GetYaxis()->GetXmin();
  xMax =  histIn->GetXaxis()->GetXmax();
  yMax =  histIn->GetYaxis()->GetXmax(); 
 
/// Calculation of the mask dimensions  
  binWidthX = (xMax-xMin)/(double)xBins;
  binWidthY = (yMax-yMin)/(double)yBins;   
  
  int maskWidthX = (6.0*sigmaX/binWidthX)/2; 
  int maskWidthY = (6.0*sigmaY/binWidthY)/2; 
  
  int maskHalfWidthX = maskWidthX;
  int maskHalfWidthY = maskWidthY;

  xTemp = -(maskWidthX+0.5)*binWidthX;
  yTemp = -(maskWidthY+0.5)*binWidthY;
  
  maskWidthX*=2;
  maskWidthY*=2;
  
  maskWidthX+=1;
  maskWidthY+=1;
  
/// End of calculation

  double mask[maskWidthX][maskWidthY];
  double maskX[maskWidthX];
  double maskY[maskWidthY];
  
  double norm = 0.0;

  cout<<"maskHalfWidthX "<<maskHalfWidthX<<" ";
  cout<<"maskHalfWidthY "<<maskHalfWidthY<<endl;  
  cout<<"maskWidthX "<<maskWidthX<<" ";
  cout<<"maskWidthY "<<maskWidthY<<endl;  

/// Start of the mask generation
  cout<<"Mask X:"<<endl;
  for ( int i = 0; i < maskWidthX; i++) // 1D Gaussian in X
  {
    maskX[i] = -erf((xTemp/(sqrt(2.0)*sigmaX)));
    cout<<i<<" "<<erf((xTemp/(sqrt(2.0)*sigmaX)))<<" ";
    xTemp+=binWidthX;
    maskX[i]+= erf((xTemp/(sqrt(2.0)*sigmaX))); 
    cout<<erf((xTemp/(sqrt(2.0)*sigmaX)))<<" "<<maskX[i]<<endl;  
  }

  cout<<endl<<"Mask Y:"<<endl;  
  for ( int i = 0; i < maskWidthY; i++) // 1D Gaussian in Y
  {
    maskY[i] = -erf((yTemp/(sqrt(2.0)*sigmaY)));
    cout<<i<<" "<<erf((yTemp/(sqrt(2.0)*sigmaY)))<<" ";    
    yTemp+=binWidthY;
    maskY[i]+= erf((yTemp/(sqrt(2.0)*sigmaY)));
    cout<<erf((yTemp/(sqrt(2.0)*sigmaY)))<<" "<<maskY[i]<<endl;       
  }
  
  for ( int i = 0; i < maskWidthX; i++) // 2D Gaussian
  {
    for ( int j = 0; j < maskWidthY; j++)
    { 
      mask[i][j] = maskX[i]*maskY[j]; // Convolution of both
      norm+=mask[i][j]; // Normalisation factor
      cout<<mask[i][j]<<" ";
    }
    cout<<endl;
  }    
/// End of mask generation   

  TH2D* histOut = (TH2D*)histIn->Clone("clonehistin");  
  TH2D* hbcg = (TH2D*)bcg->Clone("clonebcg");
  
  int m = 0;
  int n = 0;
  int xtemp = 0;
  int ytemp = 0;  

  double error = 0.0;
  double counts = 0.0;

  for(int i = 0; i<xBins; i++)
  {    
    for(int j = 0; j<yBins; j++)
    {
      ytemp = j - maskHalfWidthY;
      xtemp = i - maskHalfWidthX; 
    
      for (m = 0; m < maskWidthX; m++)
      {
        for (n = 0; n < maskWidthY; n++)
        {
          counts += hbcg->GetBinContent(xtemp, ytemp)*mask[m][n]; // weighted sum of the counts in the background
          error += pow(hbcg->GetBinError(xtemp, ytemp)*mask[m][n], 2.0); // uncertainty calculation
          ytemp++;
        }    
        xtemp++;
      }  
      
      counts/=norm; // normalisation
      error/=norm*norm;
      
      histOut->SetBinContent(i,j, histOut->GetBinContent(i,j)-counts*weight); // subtraction
      histOut->GetBinError(i,j, sqrt(histOut->GetBinError(i,j)*histOut->GetBinError(i,j)+error*weight*weight));
                  
      counts = 0.0;
      error = 0.0;
    }      
  }
  
  histOut->SetName(histIn->GetName());
  histOut->SetTitle(histIn->GetTitle());  
  
  return histOut;
}

/** Matrix subtraction. A region of 2D Gaussian in background matrix 
 * is searched for events to be subracted. **/
TH2D* backgroundSubtraction(TH2D* histIn, TH2D* bcg, double weight, double sigmaX, double sigmaY)
{
  int xBins, yBins;
  double xMin, xMax, yMin, yMax;

  double xTemp = 0.0;
  double yTemp = 0.0;
  double binWidthX;
  double binWidthY;
  double difference = 0.0;
  double bcgCounts;
  
  TRandom3 rnd(0);
  
  i_vector limitX;
  int limitXTemp;
  
  if( checkRanges( histIn, bcg) == 0) return 0; // checks, if the matrices have the same X and Y axes. 
  
  xBins =  histIn->GetNbinsX();
  yBins =  histIn->GetNbinsY();
  xMin =  histIn->GetXaxis()->GetXmin();
  yMin =  histIn->GetYaxis()->GetXmin();
  xMax =  histIn->GetXaxis()->GetXmax();
  yMax =  histIn->GetYaxis()->GetXmax(); 
  
  binWidthX = (xMax-xMin)/(double)xBins;
  binWidthY = (yMax-yMin)/(double)yBins;   
 
/// Generation of the shape of the region to be searched 
  for(yTemp=0.5*binWidthY; (xTemp*xTemp)/(sigmaX*sigmaX)+(yTemp*yTemp)/(sigmaY*sigmaY)<3.0*3.0; yTemp+=binWidthY)
  {
    limitXTemp = 1;
    for(xTemp=0.5*binWidthX; (xTemp*xTemp)/(sigmaX*sigmaX)+(yTemp*yTemp)/(sigmaY*sigmaY)<3.0*3.0; xTemp+=binWidthX) 
    {
      limitXTemp++;         
    }   
    limitX.push_back(limitXTemp);
    xTemp=0.5*binWidthX;
  }
/// End of region generation
  
  TH2D* histOut = (TH2D*)histIn->Clone("clonehistin");  
  TH2D* hbcg = (TH2D*)bcg->Clone("clonebcg");
  
  int m = 0;
  int n = 0;
  int p = 0;
  int q = 0;  
  int r = limitX.size() < limitX[0] ? limitX.size() : limitX[0];
  double error = 0.0;
  double prevVal = 0.0;
  double counts = 0.0;

  for(int i = 0; i<xBins; i++)
  {
    for(int j = 0; j<yBins; j++)
    {      
      /// Search where the counts can be subtracted. It goes counter-clockwise, spiraling out from (i,j) bin.
      for(int k = 0; k < r; k++)// increasing radius
      {
        // four loops for four quadrants. The iterators differ, the algrithm is the same
        for(m=k, n=0; n < k+1; m--, n++) // I quadrant
        {
          if( m > limitX.size() || n > limitX[m] ) continue;
          if( hbcg->GetBinContent(i, j) == 0 ) break; // no more counts in background 
          
          if( histOut->GetNbinsX() <= n ) break;
          if( histOut->GetNbinsY() <= m ) break;            
                        
          difference = histOut->GetBinContent(i+n, j+m) - weight*(double)hbcg->GetBinContent(i, j);
          error = histOut->GetBinError(i+n, j+m)*histOut->GetBinError(i+n, j+m);
          prevVal = histOut->GetBinContent(i+n, j+m);        
  
          if(difference >= 0.0) //all the counts in the background can be subtracted for this bin 
          {
            histOut->SetBinContent(i+n, j+m, difference); 
            hbcg->SetBinContent(i, j, 0);   
            break;       
          }
          else // there are more counts in the background than in this bin. 
          {
            histOut->SetBinContent(i+n, j+m, 0);  
            hbcg->SetBinContent(i, j, -difference/weight); //subtract the number of the counts in the background bin        
          }
          error+= (prevVal - histOut->GetBinContent(i+n, j+m)); //recalculate the uncertainty
          error = sqrt(error);
          histOut->SetBinError(i+n, j+m, error);                         
        }
        
        for( n = n-2 ; n > 0; m--, n--) // IV quadrant
        {        
          if( -m > limitX.size() || n > limitX[-m] ) continue;
          if( hbcg->GetBinContent(i, j) == 0 ) break;  

          if( i+n < 0 ) break;
          if( j+m < 0 ) break;            
                        
          difference = histOut->GetBinContent(i+n, j+m) - weight*(double)hbcg->GetBinContent(i, j);
          error = histOut->GetBinError(i+n, j+m)*histOut->GetBinError(i+n, j+m);
          prevVal = histOut->GetBinContent(i+n, j+m);       
  
          if(difference >= 0.0)
          {
            histOut->SetBinContent(i+n, j+m, difference); 
            hbcg->SetBinContent(i, j, 0);   
            break;       
          }
          else
          {
            histOut->SetBinContent(i+n, j+m, 0);  
            hbcg->SetBinContent(i, j, -difference/weight);         
          }
          error+= (prevVal - histOut->GetBinContent(i+n, j+m));
          error = sqrt(error);
          histOut->SetBinError(i+n, j+m, error);                       
        } 
        
        for( m = m+2 ; -n < k+1; m++, n--) // III quadrant
        {      
          if( -m > limitX.size() || -n > limitX[-m] ) continue;
          if( hbcg->GetBinContent(i, j) == 0 ) break;  

          if( i+n < 0 ) break;
          if( j+m < 0 ) break;            
                        
          difference = histOut->GetBinContent(i+n, j+m) - weight*(double)hbcg->GetBinContent(i, j);
          error = histOut->GetBinError(i+n, j+m)*histOut->GetBinError(i+n, j+m);
          prevVal = histOut->GetBinContent(i+n, j+m);        
  
          if(difference >= 0.0)
          {
            histOut->SetBinContent(i+n, j+m, difference); 
            hbcg->SetBinContent(i, j, 0);   
            break;       
          }
          else
          {
            histOut->SetBinContent(i+n, j+m, 0);  
            hbcg->SetBinContent(i, j, -difference/weight);         
          }
          error+= (prevVal - histOut->GetBinContent(i+n, j+m));
          error = sqrt(error);
          histOut->SetBinError(i+n, j+m, error);                        
        }
        for( n = n+2 ; n < 0; m++, n++) // II quadrant
        {
          ///cout<<m<<" "<<n<<endl;          
          if( m > limitX.size() || -n > limitX[-m] ) continue;
          if( hbcg->GetBinContent(i, j) == 0 ) break;  

          if( i+n < 0 ) break;
          if( j+m < 0 ) break;            
                        
          difference = histOut->GetBinContent(i+n, j+m) - weight*(double)hbcg->GetBinContent(i, j);
          error = histOut->GetBinError(i+n, j+m)*histOut->GetBinError(i+n, j+m);
          prevVal = histOut->GetBinContent(i+n, j+m);         
  
          if(difference >= 0.0)
          {
            histOut->SetBinContent(i+n, j+m, difference); 
            hbcg->SetBinContent(i, j, 0);   
            break;       
          }
          else
          {
            histOut->SetBinContent(i+n, j+m, 0);  
            hbcg->SetBinContent(i, j, -difference/weight);         
          }
          error+= (prevVal - histOut->GetBinContent(i+n, j+m));
          error = sqrt(error);
          histOut->SetBinError(i+n, j+m, error);                      
        }                                
        if(hbcg->GetBinContent(i, j)==0)
          break;                 
      }      
    }      
  }
  histOut->SetName(histIn->GetName());
  histOut->SetTitle(histIn->GetTitle());  
  
  return histOut;
}


/** This function checks, if the matrices have the same X and Y axes. 
 * The background subtraction methods work on bin numbers, not their values,
 * so if the axes are different, they won't work correctly.
*/
int checkRanges(TH2D* histo, TH2D* bcg)
{
  int errors = 1;
  if(histo==0 || histo->GetEntries()==0)
  {
    cout<<"data matrix is empty"<<endl;
    errors = 0;
  }

  if(bcg==0 || bcg->GetEntries()==0)
  {
    cout<<"background matrix is empty"<<endl;
    errors =  0;
  }  
  
  if(histo->GetNbinsX() != bcg->GetNbinsX())
  {
    cout<<"Number of X bins differ"<<endl;
    errors = 0;
  }  
  
  if(histo->GetNbinsY() != bcg->GetNbinsY())
  {
    cout<<"Number of Y bins differ"<<endl;
    errors = 0; 
  }  

  if(histo->GetXaxis()->GetXmin() != bcg->GetXaxis()->GetXmin())
  {
    cout<<"X minimum values differ"<<endl;
    errors = 0;
  }
  
  if(histo->GetYaxis()->GetXmin() != bcg->GetYaxis()->GetXmin())
  {
    cout<<"Y maximum values differ"<<endl;
    errors = 0;
  }   
  
  if(histo->GetXaxis()->GetXmax() != bcg->GetXaxis()->GetXmax())
  {
    cout<<"X maximum values differ"<<endl;
    errors = 0;
  }  
  
  if(histo->GetYaxis()->GetXmax() != bcg->GetYaxis()->GetXmax())
  {
    cout<<"Y maximum values differ"<<endl;
    errors = 0;
  }   
  
  return errors;
}
