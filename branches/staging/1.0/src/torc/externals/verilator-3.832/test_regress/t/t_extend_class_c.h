// -*- C++ -*-
// DESCRIPTION: Verilator: Verilog Test module
//
// This file ONLY is placed into the Public Domain, for any use,
// without warranty, 2006-2007 by Wilson Snyder.

class t_extend_class_c {
public:
    // CONSTRUCTORS
    t_extend_class_c() {}
    ~t_extend_class_c() {}
    // METHODS
    // This function will be called from a instance created in Verilog
    inline vluint32_t my_math(vluint32_t in) {
	return in+1;
    }
};