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
#include <mpl.h>

int main(int argc, char *argv[])
{
	mpl::MTime etime;			// demostrate how to use mtime

    std::cout << mpl::now() << std::endl;
	mpl::MDateTime datetime = mpl::MDateTime::currentDateTime();
	std::cout << datetime.year() << std::endl;
	std::cout << datetime.dayOfWeek() << std::endl;
	std::cout << datetime.dayOfYear() << std::endl;
	std::cout << datetime.toString() << std::endl;
	std::cout << datetime << std::endl;

	sleep(1);
	datetime = time(NULL);
	std::cout << datetime << std::endl;
	std::cout << datetime.data() << std::endl;

	mpl::MDateTime dt = 0x7FFFFFFF;
	std::cout << dt << std::endl;

	mpl::MDateTime dtnow;
	mpl::MDateTime s = dtnow.addDays(27);
	printf("addDays(27) testing: %s\n", s.toString().c_str());
	printf("%s wday = %d\n", s.toString().c_str(), s.dayOfWeek());

	mpl::MDateTime yund = "20190101";
	std::cout << yund.isValid() << std::endl;
	int days = dtnow.daysTo(yund);
	printf("daysTo(%s): %d\n", yund.toString().c_str(), days);

	mpl::MDateTime week = std::string("2019-01-07");
	week = "2018-10-09 12:00:00";
	std::cout << week.isValid() << std::endl;
	std::cout << week.dayOfWeek() << std::endl;
	std::cout << week.toString("%Y%m%d") << std::endl;

	std::cout << mpl::MDateTime().toString("%Y%m%d") << std::endl;
	std::cout << mpl::MDateTime("20180917").toString("%Y-%m-%d") << std::endl;

	if (week < yund) {
		printf("'%s' daysTo '%s' is %d\n",
			   week.toString("%Y-%m-%d").c_str(),
			   yund.toString("%Y-%m-%d").c_str(),
			   week.daysTo(yund));
	} else {
		printf("'%s' daysTo '%s' is %d\n",
			   yund.toString("%Y-%m-%d").c_str(),
			   week.toString("%Y-%m-%d").c_str(),
			   yund.daysTo(week));
	}
	printf("elapsed time: %fs\n", etime.elapsed());
    return 0;
}
