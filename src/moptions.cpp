/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <mpl/moptions.h>
#include <mpl/mapplication.h>
#include <mpl/mstring.h>
#include <assert.h>

MPL_BEGIN_NAMESPACE

static void fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	// fprintf(stderr, "%s: ", applicationName().c_str());
	vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

MOptions::MOptions(const char *version, const char *description)
{
	_version = version;
	_desc = description;
}

MOptions::~MOptions()
{
}

void MOptions::insert(int key, const std::string &longopt, const std::string &desc, bool reqArg)
{
	struct option_t item = {key, longopt, desc, reqArg};
	_options.push_back(item);
}

bool MOptions::find(int key) const
{
	return (_values.find(key) != _values.end());
}

int MOptions::getint(int key) const
{
	return strtol(value(key).c_str(), NULL, 10);
}

std::string MOptions::getstring(int key) const
{
	return value(key);
}

std::string MOptions::value(int key) const
{
	values_const_iterator it = _values.find(key);
	assert(it != _values.end());
	return it->second;
}

void MOptions::help() const
{
	printf("Usage: %s [OPTION] ...\n", applicationName().c_str());
	printf("%s\n", _desc.empty() ? "Program description" : _desc.c_str());
	printf("\n");
	printf("  -h, --help\t\t\tDisplay this help and exit\n");
	printf("  -V, --version\t\t\tOutput version information and exit\n");
	printf("  -c, --config=FILE\t\tUsing FILE as configuration file\n");
	printf("  -v, --verbose\t\t\tVerbose mode, explain what is being done\n");
	for (options_const_iterator it = _options.begin(); it != _options.end();
		 ++it) {
		printf("  -%c, --%s\t\t\t%s\n",
			   it->key, it->longopt.c_str(), it->desc.c_str());
	}
}

void MOptions::version() const
{
	printf("%s %s\n", applicationName().c_str(), _version.c_str());
}

void MOptions::parse(int argc, char *argv[])
{
	std::vector<struct option> longopts;
	std::string shortopts = "hVc:v";

	struct option opt_h = {"help",      no_argument,       NULL, 'h'};
	struct option opt_V = {"version",   no_argument,       NULL, 'V'};
	struct option opt_c = {"config",    required_argument, NULL, 'c'};
	struct option opt_v = {"verbose",   no_argument,       NULL, 'v'};
	longopts.push_back(opt_h);
	longopts.push_back(opt_V);
	longopts.push_back(opt_c);
	longopts.push_back(opt_v);
			
	for (options_const_iterator it = _options.begin(); it != _options.end();
		 ++it) {
		struct option item = {
			it->longopt.c_str(),
			(int)it->req_arg,
			NULL,
			it->key
		};
		longopts.push_back(item);

		shortopts += it->req_arg ?
			mpl::format("%c:", it->key) :
			mpl::format("%c", it->key);
	}

	int opt = -1;
	int longind = 0;
	while ((opt = getopt_long(argc, argv, shortopts.c_str(),
							  longopts.data(), &longind)) >= 0) {

		switch (opt) {
		case 'h':
			help();
			exit(EXIT_SUCCESS);
		case 'V':
			version();
			exit(EXIT_SUCCESS);
		case 'v':
			_values[opt] = "";
			continue;
		case 'c':
			_values[opt] = optarg;
			continue;
		default:
			break;
		}

		options_const_iterator it = std::find(
			_options.begin(), _options.end(), opt);

		if (it != _options.end()) {
			if (it->req_arg && optarg) {
				_values[opt] = optarg;
			} else {
				_values[opt] = "";
			}
		} else {
			fatal("Try '%s --help' for more information.", applicationName().c_str());
		}
	}
}

MPL_END_NAMESPACE
