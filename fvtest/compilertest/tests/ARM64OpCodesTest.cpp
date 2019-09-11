/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "compile/Method.hpp"
#include "env/jittypes.h"
#include "gtest/gtest.h"

#include "tests/ARM64OpCodesTest.hpp"

namespace TestCompiler
{


void
ARM64OpCodesTest::compileBitwiseTestMethods()
   {
   int32_t rc;

   compileOpCodeMethod(_iAnd, _numberOfBinaryArgs, TR::iand, "iAnd", _argTypesBinaryInt, TR::Int32, rc);
   compileOpCodeMethod(_iOr, _numberOfBinaryArgs, TR::ior, "iOr", _argTypesBinaryInt, TR::Int32, rc);
   compileOpCodeMethod(_iXor, _numberOfBinaryArgs, TR::ixor, "iXor", _argTypesBinaryInt, TR::Int32, rc);

   compileOpCodeMethod(_lAnd, _numberOfBinaryArgs, TR::land, "iAnd", _argTypesBinaryLong, TR::Int64, rc);
   compileOpCodeMethod(_lOr, _numberOfBinaryArgs, TR::lor, "iOr", _argTypesBinaryLong, TR::Int64, rc);
   compileOpCodeMethod(_lXor, _numberOfBinaryArgs, TR::lxor, "iXor", _argTypesBinaryLong, TR::Int64, rc);
   }

void
ARM64OpCodesTest::invokeBitwiseTests()
   {
   int32_t rc = 0;
   char resolvedMethodName [RESOLVED_METHOD_NAME_LENGTH];
   uint32_t testCaseNum = 0;

   int32_t intAndOperand1 = -1;
   int32_t intAndOperand2Arr [] =
      {
      /* element size = 32 */
      static_cast<int32_t>(0x7fffffff), // imms = 31, rotate count = 0
      static_cast<int32_t>(0xffff7fff), // imms = 31, rotate count = 16
      static_cast<int32_t>(0xfffffffe), // imms = 31, rotate count = 31
      static_cast<int32_t>(0x3fffffff), // imms = 30, rotate count = 0
      static_cast<int32_t>(0xffff3fff), // imms = 30, rotate count = 16
      static_cast<int32_t>(0xfffffffc), // imms = 30, rotate count = 30
      static_cast<int32_t>(0x7ffffffe), // imms = 30, rotate count = 31
      static_cast<int32_t>(0x0000ffff), // imms = 16, rotate count = 0
      static_cast<int32_t>(0xff0000ff), // imms = 16, rotate count = 8
      static_cast<int32_t>(0xffff0000), // imms = 16, rotate count = 16
      static_cast<int32_t>(0x00ffff00), // imms = 16, rotate count = 24
      //TODO imms = 8
      static_cast<int32_t>(0x00000003), // imms = 2, rotate count = 0
      static_cast<int32_t>(0x80000001), // imms = 2, rotate count = 1
      static_cast<int32_t>(0xc0000000), // imms = 2, rotate count = 2
      static_cast<int32_t>(0x00030000), // imms = 2, rotate count = 16
      /* element size = 16 */
      static_cast<int32_t>(0x7fff7fff), // imms = 16, rotate count = 0
      static_cast<int32_t>(0xff7fff7f), // imms = 16, rotate count = 8
      static_cast<int32_t>(0xfffefffe), // imms = 16, rotate count = 15
      static_cast<int32_t>(0x3fff3fff), // imms = 15, rotate count = 0
      static_cast<int32_t>(0xff3fff3f), // imms = 15, rotate count = 8
      static_cast<int32_t>(0xfffcfffc), // imms = 15, rotate count = 14
      static_cast<int32_t>(0x7ffe7ffe), // imms = 15, rotate count = 15
      static_cast<int32_t>(0x00ff00ff), // imms = 8, rotate count = 0
      static_cast<int32_t>(0xf00ff00f), // imms = 8, rotate count = 4
      static_cast<int32_t>(0xff00ff00), // imms = 8, rotate count = 8
      static_cast<int32_t>(0x0ff00ff0), // imms = 8, rotate count = 12
      static_cast<int32_t>(0x00030003), // imms = 2, rotate count = 0
      static_cast<int32_t>(0x80018001), // imms = 2, rotate count = 1
      static_cast<int32_t>(0xc000c000), // imms = 2, rotate count = 2
      static_cast<int32_t>(0x03000300), // imms = 2, rotate count = 8

      static_cast<int32_t>(0x80),
      static_cast<int32_t>(0xff),
      };

   signatureCharII_I_testMethodType * iBitwiseConst = 0;

   //verify that iandimmw is correctly encoded
   testCaseNum = sizeof(intAndOperand2Arr) / sizeof(intAndOperand2Arr[0]);
   for (int32_t i = 0 ; i < testCaseNum ; i++)
      {
      OMR_CT_EXPECT_EQ(_iAnd, tand(intAndOperand1, intAndOperand2Arr[i]), _iAnd(intAndOperand1, intAndOperand2Arr[i]));

      sprintf(resolvedMethodName, "iAndConst_TestCase%d", i + 1);
      compileOpCodeMethod(iBitwiseConst, 
            _numberOfBinaryArgs, TR::iand, resolvedMethodName, _argTypesBinaryInt, TR::Int32, rc, 2, 2, &(intAndOperand2Arr[i]));
      OMR_CT_EXPECT_EQ(iBitwiseConst, tand(intAndOperand1, intAndOperand2Arr[i]), iBitwiseConst(intAndOperand1, INT_PLACEHOLDER_2));
     }

   }

#if defined(TR_TARGET_ARM64)

TEST(JITARM64OpCodesTest, BitwiseTest)
   {
   ::TestCompiler::ARM64OpCodesTest ARM64BitwiseTest;
   ARM64BitwiseTest.compileBitwiseTestMethods();
   ARM64BitwiseTest.invokeBitwiseTests();
   }

#endif

}
