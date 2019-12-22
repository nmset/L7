--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: list1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE list1 (
    id_list1 integer NOT NULL,
    item character varying(50) DEFAULT (NOT NULL::boolean)
);


ALTER TABLE public.list1 OWNER TO postgres;

--
-- Name: list1_id_list1_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE list1_id_list1_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.list1_id_list1_seq OWNER TO postgres;

--
-- Name: list1_id_list1_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE list1_id_list1_seq OWNED BY list1.id_list1;


--
-- Name: tbl1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tbl1 (
    pk1 integer NOT NULL,
    text1 character varying(255) NOT NULL,
    check1d integer DEFAULT 0,
    check1t integer,
    combo1t integer,
    combo1x character varying(50),
    date1 date,
    spin1 integer DEFAULT 3,
    rand1 integer DEFAULT round((random() * (10000000)::double precision)) NOT NULL,
    json1 jsonb,
    xml1 text
);


ALTER TABLE public.tbl1 OWNER TO postgres;

--
-- Name: tbl1_pk1_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tbl1_pk1_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tbl1_pk1_seq OWNER TO postgres;

--
-- Name: tbl1_pk1_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tbl1_pk1_seq OWNED BY tbl1.pk1;


--
-- Name: tbl2; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tbl2 (
    pk2 integer NOT NULL,
    pk1 integer NOT NULL,
    text2 character varying(255) NOT NULL,
    check2d integer DEFAULT 0,
    check2t integer,
    combo2t integer,
    combo2x character varying(50),
    date2 date,
    spin2 integer DEFAULT 5,
    rand1 integer,
    json2 jsonb,
    xml2 text
);


ALTER TABLE public.tbl2 OWNER TO postgres;

--
-- Name: tbl2_pk2_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tbl2_pk2_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tbl2_pk2_seq OWNER TO postgres;

--
-- Name: tbl2_pk2_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tbl2_pk2_seq OWNED BY tbl2.pk2;


--
-- Name: id_list1; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY list1 ALTER COLUMN id_list1 SET DEFAULT nextval('list1_id_list1_seq'::regclass);


--
-- Name: pk1; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tbl1 ALTER COLUMN pk1 SET DEFAULT nextval('tbl1_pk1_seq'::regclass);


--
-- Name: pk2; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tbl2 ALTER COLUMN pk2 SET DEFAULT nextval('tbl2_pk2_seq'::regclass);


--
-- Data for Name: list1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY list1 (id_list1, item) FROM stdin;
1	List1Item1
2	List1Item2
3	List1Item3
4	List1Item4
5	List1Item5
\.


--
-- Name: list1_id_list1_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('list1_id_list1_seq', 5, true);


--
-- Data for Name: tbl1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tbl1 (pk1, text1, check1d, check1t, combo1t, combo1x, date1, rand1) FROM stdin;
\.


--
-- Name: tbl1_pk1_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('tbl1_pk1_seq', 0, true);


--
-- Data for Name: tbl2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tbl2 (pk2, pk1, text2, check2d, check2t, combo2t, combo2x, date2, rand1) FROM stdin;
\.


--
-- Name: tbl2_pk2_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('tbl2_pk2_seq', 0, true);


--
-- Name: list1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY list1
    ADD CONSTRAINT list1_pkey PRIMARY KEY (id_list1);


--
-- Name: tbl1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tbl1
    ADD CONSTRAINT tbl1_pkey PRIMARY KEY (pk1);


--
-- Name: tbl2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tbl2
    ADD CONSTRAINT tbl2_pkey PRIMARY KEY (pk2);


--
-- Name: tbl1_combo1t_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tbl1
    ADD CONSTRAINT tbl1_combo1t_fkey FOREIGN KEY (combo1t) REFERENCES list1(id_list1);


--
-- Name: tbl2_combo2t_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tbl2
    ADD CONSTRAINT tbl2_combo2t_fkey FOREIGN KEY (combo2t) REFERENCES list1(id_list1);


--
-- Name: tbl2_pk1_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tbl2
    ADD CONSTRAINT tbl2_pk1_fkey FOREIGN KEY (pk1) REFERENCES tbl1(pk1);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- Name: list1; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE list1 FROM PUBLIC;
REVOKE ALL ON TABLE list1 FROM postgres;
GRANT ALL ON TABLE list1 TO postgres;
GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE list1 TO l7;


--
-- Name: list1_id_list1_seq; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON SEQUENCE list1_id_list1_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE list1_id_list1_seq FROM postgres;
GRANT ALL ON SEQUENCE list1_id_list1_seq TO postgres;
GRANT SELECT,UPDATE ON SEQUENCE list1_id_list1_seq TO l7;


--
-- Name: tbl1; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tbl1 FROM PUBLIC;
REVOKE ALL ON TABLE tbl1 FROM postgres;
GRANT ALL ON TABLE tbl1 TO postgres;
GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE tbl1 TO l7;


--
-- Name: tbl1_pk1_seq; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON SEQUENCE tbl1_pk1_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE tbl1_pk1_seq FROM postgres;
GRANT ALL ON SEQUENCE tbl1_pk1_seq TO postgres;
GRANT SELECT,UPDATE ON SEQUENCE tbl1_pk1_seq TO l7;


--
-- Name: tbl2; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tbl2 FROM PUBLIC;
REVOKE ALL ON TABLE tbl2 FROM postgres;
GRANT ALL ON TABLE tbl2 TO postgres;
GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE tbl2 TO l7;


--
-- Name: tbl2_pk2_seq; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON SEQUENCE tbl2_pk2_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE tbl2_pk2_seq FROM postgres;
GRANT ALL ON SEQUENCE tbl2_pk2_seq TO postgres;
GRANT SELECT,UPDATE ON SEQUENCE tbl2_pk2_seq TO l7;


--
-- PostgreSQL database dump complete
--

