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

// =========================================================================
// Arguments structure
// =========================================================================
struct Arguments {
  enum DataType { _short, _int, _float };
  std::string inputFileName;
  std::string outputFileName;
  DataType dataType;
  bool isUnsigned;
};

// =========================================================================
// DoIt Lippincott function
// =========================================================================
template <class inputT>
int DoIt(const Arguments &arguments, inputT)
{
  // =========================================================================
  // Datatype definitions
  // =========================================================================
  using ImageType = itk::Image<inputT, 3>;
  using ImageReaderType = itk::ImageFileReader<ImageType>;
  using ImageWriterType = itk::ImageFileWriter<ImageType>;

  // =========================================================================
  // Datatype definitions
  // =========================================================================
  auto imageReader = ImageReaderType::New();
  imageReader->SetFileName(arguments.inputFileName);
  imageReader->Update();

  // =========================================================================
  // Write out
  // =========================================================================
  auto imageWriter = ImageWriterType::New();
  imageWriter->SetFileName(arguments.outputFileName);
  imageWriter->SetInput(imageReader->GetOutput());
  imageWriter->Write();

  return EXIT_SUCCESS;
}

int main (int argc, char **argv)
{
  // =========================================================================
  // Parse arguments
  // =========================================================================
  //
  Arguments arguments;

  try {

    TCLAP::CmdLine cmd("itkImageConvert");

    TCLAP::ValueArg<std::string> inputInput("i", "input", "Input Image", true, "None", "string");
    TCLAP::ValueArg<std::string> outputInput("o", "output", "Output Image", true, "None", "string");
    TCLAP::ValueArg<unsigned short int> datatypeInput("d", "datatypeInput", "Datatype: (0) short, (1) int or (2) float", true, 0, "unsigned short int");
    TCLAP::SwitchArg unsignedInput("u", "unsigned", "Unsigned values", false);

    cmd.add(inputInput);
    cmd.add(outputInput);
    cmd.add(datatypeInput);
    cmd.add(unsignedInput);

    cmd.parse(argc,argv);

    arguments.inputFileName = inputInput.getValue();
    arguments.outputFileName = outputInput.getValue();
    arguments.dataType = static_cast<Arguments::DataType>(datatypeInput.getValue());
    arguments.isUnsigned = unsignedInput.getValue();

    if (arguments.dataType == Arguments::DataType::_float &&
        arguments.isUnsigned) {
      std::cerr << "Unsigned flag cannot be selected when the datatype is float" << std::endl;

      return EXIT_FAILURE;
    }

  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

  // =========================================================================
  // Call the right DoIt depending on the input arguments
  // =========================================================================
  switch (arguments.dataType)
    {
    case Arguments::DataType::_short:
      return DoIt(arguments, arguments.isUnsigned ? static_cast<unsigned short int>(0)
                                                  : static_cast<short int>(0));

    case Arguments::DataType::_int:
      return DoIt(arguments, arguments.isUnsigned ? static_cast<unsigned int>(0)
                                                  : static_cast<int>(0));

    case Arguments::DataType::_float:
      return DoIt(arguments, static_cast<float>(0));
    }

  return EXIT_SUCCESS;
}
