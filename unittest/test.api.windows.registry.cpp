// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Aug 17, 2006 - Created
//
// FIXME: Currently hard coded to NT/XP registry values to test, need other platform keys

#include <industry/config.hpp>
#include <boost/test/unit_test.hpp>

#if defined( INDUSTRY_OS_WINDOWS )
#include <industry/api/windows/registry.hpp>
//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts [ Courier New (TrueType) = COUR.TTF ]

void test_api_windows_registry() {
	using namespace industry::api::windows;

	BOOST_CHECK_EQUAL( registry[ "HKEY_LOCAL_MACHINE" ] / "SOFTWARE" / "Microsoft" / "Windows NT" / "CurrentVersion" / "Fonts" % "Courier New (TrueType)" , "COUR.TTF" );
	BOOST_CHECK_EQUAL( registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ][ "Courier New (TrueType)" ] , "COUR.TTF" );
	BOOST_CHECK_EQUAL( registry.hkey_local_machine / "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" % "Courier New (TrueType)" , "COUR.TTF" );
	BOOST_CHECK_EQUAL( registry.hkey_local_machine / "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" % "Courier New (TrueType)" , "COUR.TTF" );
	BOOST_CHECK_THROW( registry[ "HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion/Fonts/Courier New (TrueType)" ] == "COUR.TTF" , missing_registry_key ); //Forward slashes can be legal key-name components, and as such are not valid deliniators.
	BOOST_CHECK_THROW( registry[ "HKEY_LOCAL_MACHINE" ] / "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" / "Courier New (TrueType)" == "COUR.TTF" , missing_registry_key ); //We differentiate between values and keys, to prevent arbitration of semantics where a key and value of the same name both exist.

	BOOST_CHECK_EQUAL( registry[ L"HKEY_LOCAL_MACHINE" ] / L"SOFTWARE" / L"Microsoft" / L"Windows NT" / L"CurrentVersion" / L"Fonts" % L"Courier New (TrueType)" , L"COUR.TTF" );
	BOOST_CHECK_EQUAL( registry[ L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ][ L"Courier New (TrueType)" ] , L"COUR.TTF" );
	BOOST_CHECK_EQUAL( registry.hkey_local_machine / L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" % L"Courier New (TrueType)" , L"COUR.TTF" );
	BOOST_CHECK_EQUAL( registry.hkey_local_machine / L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" % L"Courier New (TrueType)" , L"COUR.TTF" );
	BOOST_CHECK_THROW( registry[ L"HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion/Fonts/Courier New (TrueType)" ] == L"COUR.TTF" , missing_registry_key ); //Forward slashes can be legal key-name components, and as such are not valid deliniators.
	BOOST_CHECK_THROW( registry[ L"HKEY_LOCAL_MACHINE" ] / L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" / L"Courier New (TrueType)" == L"COUR.TTF" , missing_registry_key ); //We differentiate between values and keys, to prevent arbitration of semantics where a key and value of the same name both exist.

	registry_key_reference fonts = registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ];
	BOOST_CHECK_EQUAL( find_value( fonts , "Courier New" ).size() , 4u );
	BOOST_CHECK_EQUAL( find_value( fonts , "Courier" ).size() , 5u );
}

#undef CHECK_NO_THROW_AND_EQUAL

#else //!defined( INDUSTRY_OS_WINDOWS )
void test_api_windows_registry() {
	BOOST_ERROR( "INDUSTRY_OS_WINDOWS not defined but test run anyways - unable to test" );
}

#endif
