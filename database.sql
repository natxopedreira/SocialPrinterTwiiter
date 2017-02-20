CREATE DATABASE  IF NOT EXISTS `socialPrinter` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_bin */;
USE `socialPrinter`;
-- MySQL dump 10.13  Distrib 5.7.17, for Linux (x86_64)
--
-- Host: localhost    Database: socialPrinter
-- ------------------------------------------------------
-- Server version	5.7.17-0ubuntu0.16.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `posiciones`
--

DROP TABLE IF EXISTS `posiciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `posiciones` (
  `idposiciones` int(11) NOT NULL AUTO_INCREMENT,
  `id` int(11) DEFAULT NULL,
  `posx` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `posy` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `ancho` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `alto` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `ocupado` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `twitt` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `posXprint` varchar(45) COLLATE utf8_bin DEFAULT '0',
  `posYprint` varchar(45) COLLATE utf8_bin DEFAULT '0',
  PRIMARY KEY (`idposiciones`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `posiciones`
--

LOCK TABLES `posiciones` WRITE;
/*!40000 ALTER TABLE `posiciones` DISABLE KEYS */;
/*!40000 ALTER TABLE `posiciones` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `twits`
--

DROP TABLE IF EXISTS `twits`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `twits` (
  `idtwits` int(11) NOT NULL AUTO_INCREMENT,
  `twitId` varchar(25) COLLATE utf8_bin DEFAULT NULL,
  `urlPhoto` varchar(500) COLLATE utf8_bin DEFAULT NULL,
  `destPhoto` varchar(500) COLLATE utf8_bin DEFAULT NULL,
  `procesado` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`idtwits`),
  UNIQUE KEY `idtwits_UNIQUE` (`idtwits`),
  UNIQUE KEY `twit_ID_UNIQUE` (`twitId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `twits`
--

LOCK TABLES `twits` WRITE;
/*!40000 ALTER TABLE `twits` DISABLE KEYS */;
/*!40000 ALTER TABLE `twits` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-02-20 11:27:05
