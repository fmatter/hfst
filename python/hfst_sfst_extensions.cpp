namespace hfst {

// *** Wrapper variables for IOString outputs, TODO *** //

std::string hfst_sfst_output("");
std::string get_hfst_sfst_output() { return hfst::hfst_sfst_output; }
  
hfst::HfstTransducer * hfst_compile_sfst(const std::string & filename, const std::string & error_stream, bool verbose, bool output_to_console)
{
  (void)error_stream;
  (void)output_to_console;

  FILE * inputfile;
  if (filename == "")
    {
      inputfile = stdin;
    }
  else
    {
      inputfile = fopen(filename.c_str(), "rb");
      if (inputfile == NULL)
	return NULL;
    }

  bool unknown_in_use = hfst::get_unknown_symbols_in_use();
  hfst::set_unknown_symbols_in_use(false);
 
  hfst::SfstCompiler * compiler = new hfst::SfstCompiler(hfst::get_default_fst_type(), verbose);
  compiler->set_input(inputfile);
  compiler->set_filename(filename);

  try 
    {
      compiler->parse();
      hfst::set_unknown_symbols_in_use(unknown_in_use);
      if (filename != "")
	{
	  fclose(inputfile);
	}
      return compiler->get_result();
    } 
  catch (HfstException e)
    {
      hfst::set_unknown_symbols_in_use(unknown_in_use);
      printf("\nAn error happened when compiling sfst file \"%s\"\n", filename.c_str());
      return NULL;
    }
  catch(const char* p) 
    {
      hfst::set_unknown_symbols_in_use(unknown_in_use);
      printf("\nAn error happened when compiling sfst file \"%s\"\n", filename.c_str());
      return NULL;
    }
}
  
  hfst::implementations::HfstIterableTransducer hfst_read_binary_sfst_transducer(const std::string & filename)
  {
    FILE * inputfile = fopen(filename.c_str(), "rb");
    if (inputfile == NULL)
      return NULL;
    hfst::implementations::HfstIterableTransducer result = hfst::implementations::HfstIterableTransducer::read_binary_sfst_transducer(inputfile);
    fclose(inputfile);
    return result;
  }

  void hfst_write_binary_sfst_transducer(hfst::implementations::HfstIterableTransducer & fsm, const std::string & filename)
  {
    FILE * outputfile = fopen(filename.c_str(), "wb");
    if (outputfile == NULL)
      throw("hfst_write_binary_sfst_transducer failed to open file");
    fsm.write_binary_sfst_transducer(outputfile);
    fclose(outputfile);
  }
}
