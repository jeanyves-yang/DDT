#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "DanielssonDistanceTransformCLP.h"
#include "stdlib.h"
#include "itkImageIOBase.h"
#include "itkRescaleIntensityImageFilter.h"



int main(int argc, char * argv[])
{
    PARSE_ARGS;
  typedef unsigned char InputPixelType;
  typedef float OutputPixelType;
  typedef unsigned short VoronoiPixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image< InputPixelType, Dimension > InputImageType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::Image< VoronoiPixelType, Dimension > VoronoiImageType;

  typedef itk::SignedDanielssonDistanceMapImageFilter<
          InputImageType, OutputImageType, VoronoiImageType > FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType > RescalerType;
  RescalerType::Pointer scaler = RescalerType::New();

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  typedef itk::ImageFileWriter< VoronoiImageType > VoronoiWriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( input ) ;
  writer->SetFileName( output ) ;

  filter->SetInput( reader->GetOutput() ) ;
  scaler->SetInput( filter->GetOutput() ) ;
  writer->SetInput( scaler->GetOutput() ) ;

  scaler->SetOutputMaximum( 65535L ) ;
  scaler->SetOutputMinimum( 0L ) ;

  try
  {
      writer->Update();
  }
  catch( itk::ExceptionObject & exp )
  {
      std::cerr << "Exception caught !" << std::endl;
     std::cerr << exp << std::endl;
  }

  const char * voronoiMapFileName = "outputVoronoi.nrrd";

  VoronoiWriterType::Pointer voronoiWriter = VoronoiWriterType::New();
  voronoiWriter->SetFileName( voronoiMapFileName );
  voronoiWriter->SetInput( filter->GetVoronoiMap() );

  try
  {
     voronoiWriter->Update();
  }
  catch( itk::ExceptionObject & exp )
  {
     std::cerr << "Exception caught !" << std::endl;
     std::cerr << exp << std::endl;
  }

  typedef FilterType::VectorImageType OffsetImageType;
  typedef itk::ImageFileWriter< OffsetImageType > WriterOffsetType;
  WriterOffsetType::Pointer offsetWriter = WriterOffsetType::New();
  offsetWriter->SetInput( filter->GetVectorDistanceMap() ) ;
  offsetWriter->SetFileName( "offsetVectorImage.nrrd");

  try
  {
    offsetWriter->Update();
  }
  catch( itk::ExceptionObject & exp )
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << exp << std::endl;
   }

  return EXIT_SUCCESS;




}



