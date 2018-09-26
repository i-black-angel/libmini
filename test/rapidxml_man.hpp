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
#ifndef _RAPIDXML_MAN_H_
#define _RAPIDXML_MAN_H_

#include <exception>    // For std::exception

#define RAPIDXML_PARSE_ERROR(what, where) throw parse_error(what, where)
#ifndef RAPIDXML_STATIC_POOL_SIZE
    #define RAPIDXML_STATIC_POOL_SIZE (64 * 1024)
#endif

namespace rapidxml
{
	class parse_error : public std::exception
	{
	public:
		parse_error(const char *what, void *where);
		virtual const char *what() const throw();
		template<class Ch>
		Ch *where() const;
	private:
		const char *m_what;
		void *m_where;
	};
}

namespace rapidxml
{
	template<class Ch> class xml_node;
	template<class Ch> class xml_attribute;
	template<class Ch> class xml_document;

	enum node_type {
		node_document,			// A document node.
		node_element,			// An element node.
		node_data,				// A data node.
		node_cdata,				// A CDATA node.
		node_comment,			// A comment node.
		node_declaration,		// A declaration node.
		node_doctype,			// A DOCTYPE node.
		node_pi					// A PI node. Name contains target.
	};

	const int parse_no_data_nodes = 0x1;
	const int parse_no_element_values = 0x2;
	const int parse_no_string_terminators = 0x4;
	const int parse_no_entity_translation = 0x8;
	const int parse_no_utf8 = 0x10;
	const int parse_declaration_node = 0x20;
	const int parse_comment_nodes = 0x40;
	const int parse_doctype_node = 0x80;
	const int parse_pi_nodes = 0x100;
	const int parse_validate_closing_tags = 0x200;
	const int parse_trim_whitespace = 0x400;
	const int parse_normalize_whitespace = 0x800;

	const int parse_default = 0;
	const int parse_non_destructive = parse_no_string_terminators | parse_no_entity_translation;
	const int parse_fastest = parse_non_destructive | parse_no_data_nodes;
	const int parse_full = parse_declaration_node | parse_comment_nodes | parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;

	namespace internal
	{
		template<int Dummy>
		struct lookup_tables
		{
			static const uint8_t lookup_whitespace[256];
			static const uint8_t lookup_node_name[256];
			static const uint8_t lookup_text[256];
			static const uint8_t lookup_text_pure_no_ws[256];
			static const uint8_t lookup_text_pure_with_ws[256];
			static const uint8_t lookup_attribute_name[256];
			static const uint8_t lookup_attribute_data_1[256];
			static const uint8_t lookup_attribute_data_1_pure[256];
			static const uint8_t lookup_attribute_data_2[256];
			static const uint8_t lookup_attribute_data_2_pure[256];
			static const uint8_t lookup_digits[256];
			static const uint8_t lookup_upcase[256];
		};

		template<class Ch>
		inline std::size_t measure(const Ch *p);

		template<class Ch>
		inline bool compare(const Ch *p1, std::size_t size1, const Ch *p2, std::size_t size2, bool case_sensitive);
		// There's something in namespace internal 
	}

	// memory_pool
	template<class Ch = char>
	class memory_pool
	{
	public:
		typedef void *(alloc_func)(std::size_t);
		typedef void (free_func)(void *);
		memory_pool()
			: m_alloc_func(0)
			, m_free_func(0)
			{
				init();
			}

		~memory_pool()
			{
				clear();
			}

		xml_node<Ch> *allocate_node(node_type type,
									const Ch *name = 0,
									const Ch *value = 0,
									std::size_t name_size = 0,
									std::size_t value_size = 0);
		xml_attribute<Ch> *allocate_attribute(const Ch *name = 0,
											  const Ch *value = 0,
											  std::size_t name_size = 0,
											  std::size_t value_size = 0);
		Ch *allocate_string(const Ch *source = 0, std::size_t size = 0);
		xml_node<Ch> *clone_node(const xml_node<Ch> *source, xml_node<Ch> *result = 0);
		void clear();
		void set_allocator(alloc_func *af, free_func *ff);
	private:
		struct header
		{
			char *previous_begin;
		};

		void init();
		char *align(char *ptr);
		char *allocate_raw(std::size_t size);
		void *allocate_aligned(std::size_t size);

		char *m_begin;
		char *m_ptr;
		char *m_end;
		char m_static_memory[RAPIDXML_STATIC_POOL_SIZE];
		alloc_func *m_alloc_func;
		free_func *m_free_func;
	};

	// xml_base
	template<class Ch = char>
	class xml_base
	{
	public:
		xml_base()
			: m_name(0)
			, m_value(0)
			, m_parent(0)
			{}

		Ch *name() const;
		std::size_t name_size() const;
		Ch *value() const;
		std::size_t value_size() const;
		void name(const Ch *name, std::size_t size);
		void name(const Ch *name);
		void value(const Ch *value, std::size_t size);
		void value(const Ch *value);
		xml_node<Ch> *parent() const;
	protected:
		static Ch *nullstr();
		Ch *m_name;
		Ch *m_value;
		std::size_t m_name_size;
		std::size_t m_value_size;
		xml_node<Ch> *m_parent;
	};

	template<class Ch = char>
	class xml_attribute: public xml_base<Ch>
	{
		friend class xml_node<Ch>;
	public:
		xml_attribute()
			{}

		xml_document<Ch> *document() const;
		xml_attribute<Ch> *previous_attribute(const Ch *name = 0,
											  std::size_t name_size = 0,
											  bool case_sensitive = true) const;
		xml_attribute<Ch> *next_attribute(const Ch *name = 0,
										  std::size_t name_size = 0,
										  bool case_sensitive = true) const;
	private:
		xml_attribute<Ch> *m_prev_attribute;
		xml_attribute<Ch> *m_next_attribute;
	};

	template<class Ch = char>
	class xml_node: public xml_base<Ch>
	{
	public:
		xml_node(node_type type)
			: m_type(type)
			, m_first_node(0)
			, m_first_attribute(0)
			{
			}
		node_type type() const;
		xml_document<Ch> *document() const;
		xml_node<Ch> *first_node(const Ch *name = 0,
								 std::size_t name_size = 0,
								 bool case_sensitive = true) const;
		xml_node<Ch> *last_node(const Ch *name = 0,
								std::size_t name_size = 0,
								bool case_sensitive = true) const;
		xml_node<Ch> *previous_sibling(const Ch *name = 0,
									   std::size_t name_size = 0,
									   bool case_sensitive = true) const;
		xml_node<Ch> *next_sibling(const Ch *name = 0,
									   std::size_t name_size = 0,
									   bool case_sensitive = true) const;
		xml_attribute<Ch> *first_attribute(const Ch *name = 0,
										   std::size_t name_size = 0,
										   bool case_sensitive = true) const;
		xml_attribute<Ch> *last_attribute(const Ch *name = 0,
										   std::size_t name_size = 0,
										   bool case_sensitive = true) const;
		void type(node_type type);
		void prepend_node(xml_node<Ch> *child);
		void append_node(xml_node<Ch> *child);
		void insert_node(xml_node<Ch> *where, xml_node<Ch> *child);
		void remove_first_node();
		void remove_last_node();
		void remove_node(xml_node<Ch> *where);
		void remove_all_nodes();
		void prepend_attribute(xml_attribute<Ch> *attribute);
		void append_attribute(xml_attribute<Ch> *attribute);
		void insert_attribute(xml_attribute<Ch> *where, xml_attribute<Ch> *attribute);
		void remove_first_attribute();
		void remove_last_attribute();
		void remove_attribute(xml_attribute<Ch> *where);
		void remove_all_attributes();
		
	private:
		xml_node(const xml_node &);
		void operator=(const xml_node &);

		node_type m_type;
		xml_node<Ch> *m_first_node;
		xml_node<Ch> *m_last_node;
		xml_attribute<Ch> *m_first_attribute;
		xml_attribute<Ch> *m_last_attribute;
		xml_node<Ch> *m_prev_sibling;
		xml_node<Ch> *m_next_sibling;
	};

	template<class Ch = char>
	class xml_document: public xml_node<Ch>, public memory_pool<Ch>
	{
	public:
		xml_document()
			: xml_node<Ch>(node_document)
			{
			}
		
		template<int Flags>
		void parse(Ch *text);
		
		void clear();
	};

	template<class Ch>
	class node_iterator
	{
	public:
		typedef typename xml_node<Ch> value_type;
		typedef typename xml_node<Ch> &reference;
		typedef typename xml_node<Ch> *pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		node_iterator()
			: m_node(0)
			{
			}

		node_iterator(xml_node<Ch> *node)
			: m_node(node->first_node())
			{
			}

		reference operator *() const;
		pointer operator->() const;
		node_iterator &operator++();
		node_iterator operator++(int);
		node_iterator &operator--();
		node_iterator operator--(int);
		bool operator==(const node_iterator<Ch> &rhs);
		bool operator!=(const node_iterator<Ch> &rhs);
	private:
		xml_node<Ch> *m_node;
	};

	template<class Ch>
	class attribute_iterator
	{
	public:
		typedef typename xml_attribute<Ch> value_type;
		typedef typename xml_attribute<Ch> &reference;
		typedef typename xml_attribute<Ch> *pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		attribute_iterator()
			: m_attribute(0)
			{
			}

		attribute_iterator(xml_node<Ch> *node)
			: m_attribute(node->first_attribute())
			{
			}

		reference operator *() const;
		pointer operator->() const;
		attribute_iterator &operator++();
		attribute_iterator operator++(int);
		attribute_iterator &operator--();
		attribute_iterator operator--(int);
		bool operator==(const attribute_iterator<Ch> &rhs);
		bool operator!=(const attribute_iterator<Ch> &rhs);
	private:
		xml_attribute<Ch> *m_attribute;
	};

	const int print_no_indenting = 0x1;
	namespace internal
	{
		template<class OutIt, class Ch>
		inline OutIt copy_chars(const Ch *begin, const Ch *end, OutIt out);

		template<class OutIt, class Ch>
		inline OutIt copy_and_expand_chars(const Ch *begin, const Ch *end, Ch noexpand, OutIt out);

		template<class OutIt, class Ch>
		inline OutIt fill_chars(OutIt out, int n, Ch ch);

		template<class Ch, Ch ch>
		inline bool find_char(const Ch *begin, const Ch *end);

		template<class OutIt, class Ch>
		inline OutIt print_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_children(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_attributes(OutIt out, const xml_node<Ch> *node, int flags);

	    template<class OutIt, class Ch>
		inline OutIt print_data_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_cdata_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_element_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_declaration_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_comment_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_doctype_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);

		template<class OutIt, class Ch>
		inline OutIt print_pi_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
	}

	template<class OutIt, class Ch>
	inline OutIt print(OutIt out, const xml_node<Ch> &node, int flags = 0);

#ifndef RAPIDXML_NO_STREAMS
	template<class Ch>
	inline std::basic_ostream<Ch> &print(std::basic_ostream<Ch> &out, const xml_node<Ch> &node, int flags);

	template<class Ch>
    inline std::basic_ostream<Ch> &operator<<(std::basic_ostream<Ch> &out, const xml_node<Ch> &node);
#endif

	template<class Ch = char>
	class file
	{
	public:
		file(const char *filename);
		file(std::basic_istream<Ch> &stream);
		Ch *data();
		const Ch *data() const;
		std::size_t size() const;
	private:
		std::vector<Ch> m_data;
	};

	template<class Ch>
	inline std::size_t count_children(xml_node<Ch> *node);

	template<class Ch>
	inline std::size_t count_attributes(xml_node<Ch> *node);
}

#endif /* _RAPIDXML_MAN_H_ */
