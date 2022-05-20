// ITK includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

// TCLAP includes
#include <tclap/ValueArg.h>
#include <tclap/SwitchArg.h>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>

// STD includes
#include <cstdlib>

// NOTE: For now we will assume images to convert are unsigned short

int main (int argc, char **argv)
{
  // =========================================================================
  // Command-line variables
  // =========================================================================
  std::string inputFileName;
  std::string outputFileName;

  // =========================================================================
  // Parse arguments
  // =========================================================================
  try {

    TCLAP::CmdLine cmd("itkImageCompare");

    TCLAP::ValueArg<std::string> inputInput("i", "input", "Input Image ", true, "None", "string");
    TCLAP::ValueArg<std::string> outputInput("o", "output", "Output Image", true, "None", "string");

    cmd.add(inputInput);
    cmd.add(outputInput);

    cmd.parse(argc,argv);

    inputFileName = inputInput.getValue();
    outputFileName = outputInput.getValue();

  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

  // =========================================================================
  // ITK definitions
  // =========================================================================
  using ImageType = itk::Image<unsigned short, 3>;
  using ImageReaderType = itk::ImageFileReader<ImageType>;
  using ImageWriterType = itk::ImageFileWriter<ImageType>;

  // =========================================================================
  // Image conversion
  // =========================================================================
  auto inputReader = ImageReaderType::New();
  inputReader->SetFileName(inputFileName);
  inputReader->Update();

  auto outputWriter = ImageWriterType::New();
  outputWriter->SetFileName(outputFileName);
  outputWriter->Write();

  return EXIT_SUCCESS;
}
