/*
 Original file is downloaded from 
 Re: Converting histograms ROOT->PAW
 https://root.cern.ch/root/roottalk/roottalk99/2136.html
 Modified by Nobu Oct. 17, 2018
*/

FILE *fk;
static Int_t ID=0;
char aline[100];

void topaw(const char *rootFile, const char *ckumac="")
{
   // convert histograms in rootFile into a PAW macro ckumac
   // only histograms are converted
   // if ckumac is not specified assume rootFile.kumac
   // hbookFile name is rootFile with .hbook instead of .root
   // author Rene Brun
   //
   // Example
   // Root > .x topaw.C("hsimple.root")
   // will convert the Root file hsimple.root into a PAW kumac hsimple.kumac
   // To create the Hbook/Paw file, run the following PAW session
   // Paw > exec hsimple
         
   TFile f(rootFile);
   if (f.IsZombie()) {
      printf("Error invalid Rootfile name: %s\n",rootFile);
      return;
   }
   // build Hbook file name
   char hbookFile[100];
   strcpy(hbookFile,rootFile);
   char *dot = strstr(hbookFile,".");
   if (dot) strcpy(dot+1,"hbook");
   else     strcat(hbookFile,".hbook");
   // build kumac file name
   char kumac[100];
   if (strlen(ckumac) > 0) {
      strcpy(kumac,ckumac);
   } else {
      Int_t nch = strlen(rootFile);
      strcpy(kumac,rootFile);
      dot = strstr(kumac,".");
      if (dot) strcpy(dot+1,"kumac");
      else     strcat(kumac,".kumac");
   }
   fk = fopen(kumac,"w");
   if (!fk) {
      printf("Cannot open macro file:%s\n",kumac);
      return;
   }
   printf("Converting Root file: %s to PAW macro: %s\n",rootFile,kumac);
   fprintf(fk,"****************************************************\n");
   fprintf(fk,"* kumac generated by Root macro topaw.C\n");
   fprintf(fk,"* converting Root file: %s\n",f.GetName());
   fprintf(fk,"****************************************************\n");
   fprintf(fk,"*\n");
   fprintf(fk,"* Opening output Hbook file\n");
   fprintf(fk,"histo/file 22  %s ! N\n",hbookFile);
         
   // loop on all keys in the file
   TIter next(f.GetListOfKeys());
   TKey *key;
   TH1 *h;
   const char *className;
   while ((key = (TKey*)next())) {
      className = key->GetClassName();
      TClass *cl = gROOT->GetClass(className);
      if (!cl) continue;
      if (!cl->InheritsFrom("TH1")) continue;  
      h = (TH1*)key->ReadObj();
      ID++;

      printf("Converting ID= %d, h=%s, title=%s\n",ID,h->GetName(),h->GetTitle());
      fprintf(fk,"*\n");
      fprintf(fk,"*-------------------------------------\n");

      if (h->InheritsFrom("TProfile")) {
         ConvertProfile((TProfile*)h);
         delete h;
         continue;
      }               
      if (h->InheritsFrom("TH2")) {
         Convert2D(h);
         delete h;
         continue;
      }         
      Convert1D(h);
      delete h;
   }
   fprintf(fk,"close 22\n");
   fclose(fk);
}
void Convert1D(TH1 *h)
{
// Convert a Root 1-d histogram (bin contents and errors)
   TAxis *xaxis = h->GetXaxis();
   Int_t nbinsx = xaxis->GetNbins();
   fprintf(fk,"1d %d '%s' %d %g %g\n",ID,h->GetTitle(),
             nbinsx,xaxis->GetXmin(),xaxis->GetXmax());
   sprintf(aline,"Vector/Create bins(%d) R ",nbinsx);
   // convert bin contents
   Int_t bin,k;
   for (bin=1;bin<=nbinsx;bin++) {
      k = strlen(aline);
      sprintf(aline+k," %g",h->GetBinContent(bin));
      if (k > 65) {
         fprintf(fk,"%s",aline);
         if (bin < nbinsx) fprintf(fk," _\n");
         aline[0] = 0;
      }
   }
   k = strlen(aline);
   if (k > 0) fprintf(fk,"%s\n",aline);
   fprintf(fk,"\nPut/Content %d bins\n",ID); /* \n was inserted Nobu Oct. 17, 2018 */
   fprintf(fk,"Vector/Delete bins\n");
   // convert bin errors (if any)
   if (h->GetSumw2N()) {
      for (bin=1;bin<=nbinsx;bin++) {
         k = strlen(aline);
         sprintf(aline+k," %g",h->GetBinError(bin));
         if (k > 65) {
            fprintf(fk,"%s",aline);
            if (bin < nbinsx) fprintf(fk," _\n");
            aline[0] = 0;
         }
      }
      k = strlen(aline);
      if (k > 0) fprintf(fk,"%s\n",aline);
      fprintf(fk,"Put/Errors %d errors\n",ID);
      fprintf(fk,"Vector/Delete errors\n");
   }
   fprintf(fk,"hrout %d\n",ID);
}

void Convert2D(TH1 *h)
{
// convert a Root 2-d histogram (bins only)
   TAxis *xaxis = h->GetXaxis();
   TAxis *yaxis = h->GetYaxis();
   Int_t nbinsx = xaxis->GetNbins();
   Int_t nbinsy = yaxis->GetNbins();
   fprintf(fk,"2d %d '%s' %d %g %g %d %g %g\n",ID,h->GetTitle(),
          nbinsx,xaxis->GetXmin(),xaxis->GetXmax(),
          nbinsy,yaxis->GetXmin(),yaxis->GetXmax());
   sprintf(aline,"Vector/Create bins(%d) R ",nbinsx*nbinsy);
   Int_t binx,biny,k;
   for (biny=1;biny<=nbinsy;biny++) {
      for (binx=1;binx<=nbinsx;binx++) {
         k = strlen(aline);
         sprintf(aline+k," %g",h->GetCellContent(binx,biny));
         if (k > 65) {
            fprintf(fk,"%s",aline);
            if (binx < nbinsx) fprintf(fk," _\n");  /* bin -> binx Nobu Oct. 17, 2018 */
            aline[0] = 0;
         }
      }
   }
   k = strlen(aline);
   if (k > 0) fprintf(fk,"%s\n",aline);
   fprintf(fk,"\nPut/Content %d bins\n",ID);  /* \n was inserted Nobu Oct. 17, 2018 */
   fprintf(fk,"Vector/Delete bins\n");
   fprintf(fk,"hrout %d\n",ID);
}

void ConvertProfile(TProfile *h)
{
// Convert a Root Profile histogram (to be implemented)
   TAxis *xaxis = h->GetXaxis();
   Int_t nbinsx = xaxis->GetNbins();
   fprintf(fk,"Profile %d '%s' %d %g %g ! !\n",ID,h->GetTitle(),
          nbinsx,xaxis->GetXmin(),xaxis->GetXmax());
   fprintf(fk,"hrout %d\n",ID);
}
